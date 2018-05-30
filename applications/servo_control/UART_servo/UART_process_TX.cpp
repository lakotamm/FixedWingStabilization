#include "Porting_layer.hpp"
#include "UART_servo.hpp"

#define MAX_I15VALUE 16383
#define MIN_I15VALUE -16384
#define MAX_U16VALUE 65534
#define MAX_U8VALUE 254

#define MAX_SERVO_ID 14

//STARTUP_ASSERT_TX checks the sizes of structs in memory
#define STARTUP_ASSERT_TX()                                                         \
do {                                                                                \
  STATIC_SIZE_CHECK(TX_Header, TX_HEADER_LENGTH);                                   \
  STATIC_SIZE_CHECK(TX_Command, TX_COMMAND_LENGTH - TX_HEADER_LENGTH);              \
  STATIC_SIZE_CHECK(TX_Request_Message, TX_REQ_MESSAGE_LENGTH - TX_HEADER_LENGTH);  \
  STATIC_SIZE_CHECK(TX_Position, TX_POS_LENGTH - TX_HEADER_LENGTH);                 \
  STATIC_SIZE_CHECK(TX_Settings, TX_SETTINGS_LENGTH - TX_HEADER_LENGTH);            \
  STATIC_SIZE_CHECK(TX_User_String, TX_USR_STNG_LENGTH - TX_HEADER_LENGTH);         \
} while (0)

/*
* @brief Send absolute position to a UART_servo
* @param position: New position of the servo
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setAbsPosition(int16_t position){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_ABS_POS,position, FALSE);
    }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::setAbsPosition(int16_t position, bool groupControl){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_ABS_POS,position, groupControl);
    }
}

/*
* @brief Send relative position to a UART_servo
* @param position: Relative movement of the servo
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setRelPosition(int16_t position){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_REL_POS,position, FALSE);
    }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::setRelPosition(int16_t position, bool groupControl){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_REL_POS,position, groupControl);
    }
}

/*
* @brief Send relative position to a UART_servo
* @param position: Zero position of the servo
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setZeroPosition(int16_t position){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_ZERO_POS,position, FALSE);
    }
}

/*
* @brief Send relative position to a UART_servo
* @param position: Zero position of the servo
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setMinPosition(int16_t position){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_MIN_POS,position, FALSE);
    }
}

/*
* @brief Send relative position to a UART_servo
* @param position: Zero position of the servo
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setMaxPosition(int16_t position){
  if (position > MAX_I15VALUE || position < MIN_I15VALUE){
      return PARAM_OUT_OF_RANGE;
    }
  else {
      return sendPosition(TX_MAX_POS,position, FALSE);
    }
}

/*
* @brief Turn on/off the motor
* @param motor: 0 - turn motor off, >0 - turn motor on
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::setMotor(bool motor){
  if (motor){
      return sendCommand(TX_MOTOR_ON, FALSE);
    }
  else {
      return sendCommand(TX_MOTOR_OFF, FALSE);
    }
}

UARTservo::tx_result_t UARTservo::setMotor(bool motor, bool groupControl){
  if (motor){
      return sendCommand(TX_MOTOR_ON, groupControl);
    }
  else {
      return sendCommand(TX_MOTOR_OFF, groupControl);
    }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::setSettings(uint16_t pSetting, uint16_t iSetting,
  uint16_t dSetting, uint16_t currentLimit, uint16_t voltageLimit, uint8_t servo_ID,
  uint8_t group_ID, uint8_t servo_mode){
  if (pSetting > MAX_U16VALUE || iSetting > MAX_U16VALUE ||
     dSetting > MAX_U16VALUE || currentLimit > MAX_U16VALUE ||
     voltageLimit > MAX_U16VALUE || servo_ID > MAX_SERVO_ID ||
    group_ID > MAX_SERVO_ID || servo_mode > MODE_MULTIPLE_SERVOS){
    return PARAM_OUT_OF_RANGE;
    }
  else {
    return sendSettings(pSetting, iSetting, dSetting, currentLimit, voltageLimit,
    servo_ID, group_ID, servo_mode, FALSE);
    }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::setUsrString(uint8_t *userString, uint8_t length){
  if (length > TX_USER_STRING_LENGTH) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    uint8_t newUserString[TX_USER_STRING_LENGTH];
    for (size_t i = 0; i < length; i++) {
      newUserString[i]=userString[i];
    }
    for (size_t i = length; i < TX_USER_STRING_LENGTH; i++) {
      newUserString[i]=' ';
    }
    return sendUsrString(newUserString, FALSE);
  }
}

/*
* @brief Send servo a command to enter bootloader
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::enterBootloader(void){
  return sendCommand(TX_BOOTLOADER, FALSE);
}

/*
* @brief Send servo a command to reset warnings and errors
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::resetErrors(void){
  return sendCommand(TX_RST_WNG_ERR, FALSE);
}

//TODO finish comment
UARTservo::tx_result_t UARTservo::factoryReset(void){
  return sendCommand(TX_FACTORY_RST, FALSE);
}

/*
* @brief Send servo a command to write settings to EEPROM
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::writeSettingsEEPROM(void){
  return sendCommand(TX_WRT_SETTINGS, FALSE);
}

/*
* @brief Send servo a command to write user string to EEPROM
* @retval Serial status
*/
UARTservo::tx_result_t UARTservo::writeUsrStringEEPROM(void){
  return sendCommand(TX_WRT_USR_STNG, FALSE);
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::requestFeedback(uint8_t interval){
  if (interval > MAX_U8VALUE) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    return sendRqstMsg(TX_REQ_FEEDBACK, interval, FALSE);
  }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::requestServiceData(uint8_t interval){
  if (interval > MAX_U8VALUE) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    return sendRqstMsg(TX_REQ_SERVICE_DATA, interval, FALSE);
  }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::requestSettings(uint8_t interval){
  if (interval > MAX_U8VALUE) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    return sendRqstMsg(TX_REQ_SETTINGS, interval, FALSE);
  }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::requestErrors(uint8_t interval){
  if (interval > MAX_U8VALUE) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    return sendRqstMsg(TX_REQ_ERRORS, interval, FALSE);
  }
}

//TODO: finish comment
UARTservo::tx_result_t UARTservo::requestDebug(uint8_t interval){
  if (interval > MAX_U8VALUE) {
    return PARAM_OUT_OF_RANGE;
  }
  else {
    return sendRqstMsg(TX_REQ_DEBUG, interval, FALSE);
  }
}

/*
* @brief Send command message
* @param message type: Check MASTER_UART_TX_msg.h for available message types
* @retval Serial status
*/

UARTservo::tx_result_t UARTservo::sendCommand(TXMessageIDTypeDef message_type, bool group_flag){
  STARTUP_ASSERT_TX();
  clean_buffer_tx(TX_COMMAND_LENGTH);
  assign_header_tx(message_type, group_flag);

  _tx_transmit_space.message.data.command.checksum = calc_checksum(_tx_transmit_space.buffer, TX_COMMAND_LENGTH);
  return send_data(TX_COMMAND_LENGTH);
}

/*
* @brief Send request for message
* @param message type: Check MASTER_UART_TX_msg.h for available message types
* @param interval:
* @retval Serial status
*/

UARTservo::tx_result_t UARTservo::sendRqstMsg(TXMessageIDTypeDef message_type, uint8_t interval, bool group_flag){
  STARTUP_ASSERT_TX();
  clean_buffer_tx(TX_REQ_MESSAGE_LENGTH);
  assign_header_tx(message_type, group_flag);

  _tx_transmit_space.message.data.request_message.interval = interval;

  _tx_transmit_space.message.data.request_message.checksum = calc_checksum(_tx_transmit_space.buffer, TX_REQ_MESSAGE_LENGTH);
  return send_data(TX_REQ_MESSAGE_LENGTH);
}

/*
* @brief Send position message
* @param message type: Check MASTER_UART_TX_msg.h for available message types
* @param position:
* @retval Serial status
*/

UARTservo::tx_result_t UARTservo::sendPosition(TXMessageIDTypeDef message_type, int16_t position, bool group_flag){
  STARTUP_ASSERT_TX();
  clean_buffer_tx(TX_POS_LENGTH);
  assign_header_tx(message_type, group_flag);

  _tx_transmit_space.message.data.position.position = position;

  _tx_transmit_space.message.data.position.checksum = calc_checksum(_tx_transmit_space.buffer, TX_POS_LENGTH);
  return send_data(TX_POS_LENGTH);
}

//TODO finish TX_COMMAND_LENGTH
UARTservo::tx_result_t UARTservo::sendSettings(uint16_t pSetting, uint16_t iSetting,
  uint16_t dSetting, uint16_t currentLimit, uint16_t voltageLimit, uint8_t servo_ID,
  uint8_t group_ID, uint8_t servo_mode, bool group_flag){
  STARTUP_ASSERT_TX();
  clean_buffer_tx(TX_SETTINGS_LENGTH);
  assign_header_tx(TX_SETTINGS, group_flag);

  _tx_transmit_space.message.data.settings.P_setting = pSetting;
  _tx_transmit_space.message.data.settings.I_setting = iSetting;
  _tx_transmit_space.message.data.settings.D_setting = dSetting;
  _tx_transmit_space.message.data.settings.CurLim_setting = currentLimit;
  _tx_transmit_space.message.data.settings.VoltLim_setting = voltageLimit;
  _tx_transmit_space.message.data.settings.servo_ID = servo_ID;
  _tx_transmit_space.message.data.settings.group_ID = group_ID;
  _tx_transmit_space.message.data.settings.servo_mode = servo_mode;

  _tx_transmit_space.message.data.settings.checksum = calc_checksum(_tx_transmit_space.buffer, TX_SETTINGS_LENGTH);
  return send_data(TX_SETTINGS_LENGTH);
}

//TODO finish comment
UARTservo::tx_result_t UARTservo::sendUsrString(uint8_t *userString, bool group_flag){
  STARTUP_ASSERT_TX();
  clean_buffer_tx(TX_USR_STNG_LENGTH);
  assign_header_tx(TX_USR_STNG, group_flag);

  for (size_t i = 0; i < TX_USER_STRING_LENGTH; i++) {
    _tx_transmit_space.message.data.user_string.User_String[i] = userString[i];
  }

  _tx_transmit_space.message.data.user_string.checksum = calc_checksum(_tx_transmit_space.buffer, TX_USR_STNG_LENGTH);
  return send_data(TX_USR_STNG_LENGTH);
}

/*
* @brief Cleans the TX buffer
* @param message_length: The length of the message which needs to fit in the buffer
*/
void UARTservo::clean_buffer_tx(TXMessageLengthTypeDef messageLength){
    for (int var = 0; var < messageLength; ++var) {
     _tx_transmit_space.buffer[var]=0;
  }
}

//TODO finish comment
void  UARTservo::assign_header_tx(TXMessageIDTypeDef message_type, bool group_flag){
  STARTUP_ASSERT_TX();
  _tx_transmit_space.message.header.key[0] = KEY;
  _tx_transmit_space.message.header.key[1] = KEY;
  _tx_transmit_space.message.header.message_type = message_type;
  _tx_transmit_space.message.header.servo_ID = _servo_id;
  _tx_transmit_space.message.header.group_flag = group_flag;
}
