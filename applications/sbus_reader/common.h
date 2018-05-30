#ifndef __COMMON__
#define __COMMON__

// Chibios realted
#include "ch.h"

// Custom stuff
//#include "input_reader.h"

/**
 * Shared defines
 */
// Size related
#define PACKET_OVERHEAD_SIZE 1 + 1 + 2 /* UID[1] + (UID,LastData,PacketType)[1] + CRC16[2] */
#define PACKET_MAX_SIZE (PACKET_OVERHEAD_SIZE + 22)
#define PACKET_SPACING 0.5 // Multiply of the total time
#define CHANNEL_COUNT   16 /* Need to be fix to 16 for this version... */

// Rates related
#define AIR_BAUD 4800

// Time related
#define TIME_OVERHEAD (uint32_t) ((PACKET_OVERHEAD_SIZE * 8 * 1e6) / AIR_BAUD)
#define TIME_SLOT (uint32_t) ((PACKET_MAX_SIZE * 8 * 1e6) / AIR_BAUD) /* Get it in  US */
#define TIME_SPACE (uint32_t) (TIME_SLOT * PACKET_SPACING)
#define BYTES_TO_TIME(count) (uint32_t) ((count * 8 * 1e6) + TIME_OVERHEAD)  /* Calculate how long time it will take to send x bytes */
#define TIME_TO_BYTES(time) (int32_t) ((((AIR_BAUD / 8) ) * (time / 1e6)) - PACKET_OVERHEAD_SIZE) /* Calculate how many bytes we can transmit during the time in us */

// Common math functions
#define MAX(x, y)     ( ( (x) > (y) ) ? (x) : (y) )
#define MIN(x, y)     ( ( (x) < (y) ) ? (x) : (y) )
#define MIN3(x, y, z) ( ((x) < (y)) ? ((x < z) ? (x) : (z)) : ((y < z) ? (y) : (z)) )



// Maximum of 16 packates types!
typedef enum {
  /* 1 */ PKG_SYN = 1,
  /* 2 */ PKG_PAIR,
  /* 3 */ PKG_CHA_DATA,
  /* 4 */ PKG_SER_DATA,
  /* 5 */ PKG_DBG_DATA,


  PKG_INVALID = 0,
} PacketType_t;


typedef enum {
  /* 0 */ STATE_IDLE = 0,
  /* 1 */ STATE_RX,
  /* 2 */ STATE_TX,
  /* 3 */ STATE_NOT_SYNC,
  /* 4 */ STATE_BIND,
  /* 4 */ STATE_FIRST_BOOT,
  /* 4 */ STATE_DBG,
} State_t;

// Have to be compiled with -fshort-enums
typedef union
{
  unsigned char byte[3];

  struct
  {
    uint8_t uidLow          : 8;
    uint8_t uidHigh         : 6;
    uint8_t reserved1       : 2;
    PacketType_t packetType : 4;
    bool chaResend          : 1;
    bool lastData           : 1;
    uint8_t packetSpecific  : 2;
  } data;
} PacketHeader_t;


uint8_t generatePacket(PacketType_t type, bool lastData);
uint8_t generateSerialPacket(uint8_t len, bool lastData);
uint8_t readPacket(void);
uint8_t generateChannelSendList(void);
void setState(State_t newState);
void setResendChannels(void);
bool sendPacket(uint8_t length);
PacketHeader_t decodePacket(void);
uint16_t genCRC16(uint8_t *data, uint16_t size);
int randomNumber (unsigned int *seed);

/**
 * Shared variables
 */
extern volatile bool g_newPacketFlag;
// extern uint8_t g_uid;
extern uint8_t g_lqi;
extern uint8_t g_rxRssi;
extern uint8_t g_channelCount;
extern unsigned char g_rxBuffer[PACKET_MAX_SIZE + 2]; // +2 is for RSSI and CRC error flag
extern unsigned char g_txBuffer[PACKET_MAX_SIZE + 1]; // +1 is for the Length byte
extern uint16_t g_channels[CHANNEL_COUNT];
extern State_t g_state;
extern bool g_digitalChannels[2];
extern bool g_failsafe;
extern bool g_sbusFrameLost;
extern bool g_configurationMode;
//extern inputType_t g_inputType;


#endif //__COMMON__
