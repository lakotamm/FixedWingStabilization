#pragma once
#include "ch.h"


typedef struct {
  uint16_t channels[16];
  bool digitalChannels[2];
  bool failsafe;
  bool frameLost;
  bool dataReady;
} sbusData_t;


/**
 * @brief      Init the SBUS class
 */
void sbusInit(SerialDriver *driver);


/**
 * @brief      Set a single channel
 *
 * @param[in]  channel  Then channel to set 1-16
 * @param[in]  value    The value
 */
void sbusSetChannel(uint8_t channel, uint16_t value);


/**
 * @brief      Set one of the two digital channels
 *
 * @param[in]  channel  Channel 1 or 2
 * @param[in]  value    true for 1 and false for 0
 */
void sbusSetDigitalChannel(uint8_t channel, bool value);


/**
 * @brief      Set the lost frame bit
 *
 * @param[in]  value  The value
 */
void sbusSetLostFrame(bool value);


/**
 * @brief      Set the whether we are in failsafe or not
 *
 * @param[in]  value  The value
 */
void sbusSetFailsafe(bool value);


/**
 * @brief      Send the SBUS package over the UART (Need an sbus_update to contain the newest values first)
 */
void sbusSend(void);


/**
 * @brief      Update the output buffer for the SBUS
 */
void sbusUpdate(void);


/**
 * @brief      Take 1 char in at the time, when it got a full package, it will return a true.
 *
 * @param[in]  c     Single char from the UART
 *
 * @return     Return true when got a full package, else false.
 */
bool sbusDecode(uint8_t c);

uint16_t sbusGetChannel(uint8_t channel);
