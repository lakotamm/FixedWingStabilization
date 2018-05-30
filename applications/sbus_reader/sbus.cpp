// ChibiOS related
#include "ch.h"
#include "hal.h"

// Custom stuff
#include "sbus.h"
#include "common.h"
#include "debugger.hpp"

// Configuration
#define USE_SBUS_DATA_STRUCT   false         /* Not working ATM */
#define USE_INTERNAL_STOARGE   true         /* Use local variables to store the data in */
#define USE_DIGITAL_CHANNELS   true          /* Not supported to be false */
#define USE_NONE_BLOCKING      false         /* Use the sdAsynchronousWrite instead of the blocking */
#define ENABLE_DJI_SUPPORT     true          /* Send both packets with 0x0F and 0xF8 */
#define SBUS_CHANNEL_MAX       CHANNEL_COUNT /* How many channels to support (Standard S-bus is 16) */
#define SBUS_UPDATE_ON_SEND    true          /* Run the sbusSend instead of update and then send */
#define SBUS_MIN_TIME_SPACE    1000          /* Minmium space between sbus packets in US */

/* Storage configuration */
#if (!USE_INTERNAL_STOARGE)
#define SBUS_CHANNEL_ARRAY          g_channels        /* An uin16_t array with length of CHANNEL_COUNT */
#define SBUS_DIGITAL_CHANNEL_ARRAY  g_digitalChannels /* An bool array with length of 2 */
#define SBUS_FAILSAFE               g_failsafe        /* An bool variable */
#define SBUS_FRAME_LOST             g_sbusFrameLost   /* An bool variable */
#define SBUS_CHANNEL_COUNT          g_channelCount    /* Keep an count of incoming channels */
#endif

// Static variables
#if USE_INTERNAL_STOARGE
// Channel 2 is 2000 because it is throttle - set to 0 in the beginning
static uint16_t _channels[16] = {1023,1023,2000,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023};
static bool _digitalChannels[2] = {1, 1};
static bool _failsafe = false;
static bool _SBUS_FRAME_LOST = false;
static uint8_t _channelCount = 0;

// Define locale storage options
#define SBUS_CHANNEL_ARRAY         _channels
#define SBUS_DIGITAL_CHANNEL_ARRAY _digitalChannels
#define SBUS_FAILSAFE              _failsafe
#define SBUS_FRAME_LOST            _SBUS_FRAME_LOST
#define SBUS_CHANNEL_COUNT         _channelCount
#endif

// Nessesary variables
static uint8_t _dataToSend[25];
static bool _sbusReady = false;

//  static const
static volatile const uint8_t SBUS_START_BYTE_1  = 0x0F;
static volatile const uint8_t SBUS_START_BYTE_2  = 0xF8;
static volatile const uint8_t SBUS_START_BYTE_3  = 0x0F; // TODO Fix this (wrong fist bit)
static volatile const uint8_t SBUS_END_BYTE      = 0x00;
static volatile const uint8_t SBUS_CHANNEL_WIDTH = 11; // Channel size in bit's

// Stuff set under init
SerialDriver *_driver;


void sbusInit(SerialDriver *driver)
{
  // Save variables locale
  _driver = driver;

  // Init the data array to 0
  uint8_t i;
  for(i = 0 ; i < 25 ; i++)
    _dataToSend[i] = 0x00;

  _sbusReady = true;
}


uint16_t sbusGetChannel(uint8_t channel)
{
  if (channel < SBUS_CHANNEL_MAX)
    return SBUS_CHANNEL_ARRAY[channel];
  return -1;
}


bool sbusGetDigitalChannel(uint8_t channel)
{
  if (channel < 2)
    return SBUS_DIGITAL_CHANNEL_ARRAY[channel];
  return false;
}


bool sbusGetLostFrame(void)
{
  return SBUS_FRAME_LOST;
}


bool sbusGetFailsafe(void)
{
  return SBUS_FAILSAFE;
}


void sbusSetChannel(uint8_t channel, uint16_t value)
{
  if (value > 2047)
    value = 2047;
  if (channel < SBUS_CHANNEL_MAX)
    SBUS_CHANNEL_ARRAY[channel] = value;
}


void sbusSetDigitalChannel(uint8_t channel, bool value)
{
  if(channel < 2)
    SBUS_DIGITAL_CHANNEL_ARRAY[channel] = value;
}


void sbusSetLostFrame(bool value)
{
  SBUS_FRAME_LOST = value;
}


void sbusSetFailsafe(bool value)
{
  SBUS_FAILSAFE = value;
}

uint32_t tsList[26];
bool sbusDecode(uint8_t c)
{
  //dprint("%02x ", c);
  // dprintn("dc");
  static uint8_t rawData[26];
  static uint8_t p = 0;
  static uint32_t lastTime = 0;

  uint32_t curTime = chSysGetRealtimeCounterX();
  uint32_t dt = RTC2US(STM32_SYSCLK, curTime - lastTime);
  lastTime = curTime;


  // Validate that the time is okay
  if ((dt < 3000) && p ==0)
  {
    // dprintn("rt \r\n");
    return false;
  }

#if ENABLE_DJI_SUPPORT
  if (p == 0 && !(c == SBUS_START_BYTE_1 || c == SBUS_START_BYTE_2 || c == SBUS_START_BYTE_3))
    return false;
#else
  if (p == 0 && !(c == SBUS_START_BYTE_1 || c == SBUS_START_BYTE_3))
    return false;
#endif

  tsList[p] = dt;

  rawData[p++] = c;

  // Check if we should decode the data
  if (p == 25) // remember post count of p
  {
    // If invalid end byte
    if (c != SBUS_END_BYTE)
    {
      // dprintn("lala: ");
      // for(int i = 0 ; i < 25 ; i++)
      // {
      //   dprintn("%02x ", rawData[i]);
      // }
      // dprintn("\r\n");
      // dprint("End byte = %02x")
      p = 0;
      return false;
    }

    // dprintn("haha: ");
    // for(int i = 0 ; i < 25 ; i++)
    // {
    //   dprintn("%02x ", rawData[i]);
    // }
    // dprintn("\r\n");

    // We are here when we got proper start and end byte
    uint8_t ch = 0;
    uint8_t ch_bit = 0;
    uint8_t data_byte = 1; // Let data_byte start at 1 since byte 0 is the start byte
    uint8_t data_bit = 0;

    // There is always 16 channels in SBUS :'(
    SBUS_CHANNEL_COUNT = SBUS_CHANNEL_MAX;

    // Process the channel data
    uint8_t i;
    for (i = 0 ; i < SBUS_CHANNEL_MAX*11 ; i++)
    {
      if (rawData[data_byte] & (1 << data_bit))
        SBUS_CHANNEL_ARRAY[ch] |= (1 << ch_bit);
      else
        SBUS_CHANNEL_ARRAY[ch] &= ~(1 << ch_bit);

      ++ch_bit;
      ++data_bit;

      if (data_bit == 8)
      {
        data_bit = 0;
        ++data_byte;
      }
      if (ch_bit == 11)
      {
        ch_bit = 0;
        ++ch;
      }
    }

    // Process the digital channels
    if (rawData[23] & (1 << 7))
      SBUS_DIGITAL_CHANNEL_ARRAY[0] = true;
    else
      SBUS_DIGITAL_CHANNEL_ARRAY[0] = false;

    if (rawData[23] & (1 << 6))
      SBUS_DIGITAL_CHANNEL_ARRAY[1] = true;
    else
      SBUS_DIGITAL_CHANNEL_ARRAY[1] = false;

    // Process frame lost bit
    if (rawData[23] & (1 << 5))
      SBUS_FRAME_LOST = true;
    else
      SBUS_FRAME_LOST = false;

    // Process failsafe
    if (rawData[23] & (1 << 4))
      SBUS_FAILSAFE = true;
    else
      SBUS_FAILSAFE = false;

  /*  dprintn("%03d ", SBUS_DIGITAL_CHANNEL_ARRAY[0]);
    dprintn("%03d ", SBUS_DIGITAL_CHANNEL_ARRAY[1]);
    dprintn("\r\n");*/

    // Reset p and be ready to accept a new package
    p = 0;
    //palClearLine(LINE_LED_GREEN);
    return true;
  }
  return false;
}



void sbusSend(void)
{
  if (!_sbusReady)
    return;


  // Send first package
  _dataToSend[0] = SBUS_START_BYTE_1;

  dprint("Sending sbus");

  // dprintn("lala: ");
  // for(int i = 0 ; i < 25  ; i++)
  // {
  //   dprintn("%02x ", _dataToSend[i]) ;
  // }
  // dprintn("\r\n");


#if USE_NONE_BLOCKING
  sdAsynchronousWrite(_driver, _dataToSend, 25);
#else
  // sdWrite(_driver, _dataToSend, 25);
  // dprint("Sbus: ");
  // for(int i = 0 ; i < 25 ; i++)
  // {
  //   dprintn("%02x ", _dataToSend[i]);
  // }
  // dprintn("\r\n");
  sdWrite(_driver, _dataToSend, 25);
  // sdWrite(&SD2, _dataToSend, 25);
#endif // USE_NONE_BLOCKING

  // Send next package if required
#if ENABLE_DJI_SUPPORT
  _dataToSend[0] = SBUS_START_BYTE_2;
  #if USE_NONE_BLOCKING
    sdAsynchronousWrite(_driver, _dataToSend, 25);
  #else
    sdWrite(_driver, _dataToSend, 25);
  #endif // USE_NONE_BLOCKING
#endif // ENABLE_DJI_SUPPORT

}


void sbusUpdate(void)
{
  // return;
  uint8_t ch = 0;
  uint8_t ch_bit = 0;
  uint8_t data_byte = 1;
  uint8_t data_bit = 0;

  uint8_t i;

  for (i = 0 ; i < SBUS_CHANNEL_MAX*11; i++)
  {
    if ( SBUS_CHANNEL_ARRAY[ch] & (1 << ch_bit) )
      _dataToSend[data_byte] |= (1 << data_bit);
    else
      _dataToSend[data_byte] &= ~(1 << data_bit);

    ++ch_bit;
    ++data_bit;

    if (data_bit == 8)
    {
      data_bit = 0;
      ++data_byte;
    }
    if (ch_bit == 11)
    {
      ch_bit = 0;
      ++ch;
    }
  }

  // Add the digital channels
  if (SBUS_DIGITAL_CHANNEL_ARRAY[0])
    _dataToSend[23] |= (1 << 7);
  else
    _dataToSend[23] &= ~(1 << 7);

  if (SBUS_DIGITAL_CHANNEL_ARRAY[1])
    _dataToSend[23] |= (1 << 6);
  else
    _dataToSend[23] &= ~(1 << 6);

  // Add frame lost bit
  if (SBUS_FRAME_LOST)
    _dataToSend[23] |= (1 << 5);
  else
    _dataToSend[23] &= ~(1 << 5);

  // Add failsafe
  if (SBUS_FAILSAFE)
    _dataToSend[23] |= (1 << 4);
  else
    _dataToSend[23] &= ~(1 << 4);
}
