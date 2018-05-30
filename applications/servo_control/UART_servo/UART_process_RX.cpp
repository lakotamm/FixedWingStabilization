#include "Porting_layer.hpp"
#include "UART_servo.hpp"

#define REQ_NUM_OF_KEYS 2

//STARTUP_ASSERT_RX checks the sizes of structs in memory
#define STARTUP_ASSERT_RX()                                                         \
do {                                                                                \
  STATIC_SIZE_CHECK(RX_Header, RX_HEADER_LENGTH);                                   \
  STATIC_SIZE_CHECK(RX_Feedback, RX_FEEDBACK_LENGTH - RX_HEADER_LENGTH);            \
  STATIC_SIZE_CHECK(RX_Service_data, RX_SERVICE_DATA_LENGTH - RX_HEADER_LENGTH);    \
  STATIC_SIZE_CHECK(RX_Settings, RX_SETTINGS_LENGTH - RX_HEADER_LENGTH);            \
  STATIC_SIZE_CHECK(RX_Errors, RX_ERRORS_LENGTH - RX_HEADER_LENGTH);                \
  STATIC_SIZE_CHECK(RX_Debug, RX_DEBUG_LENGTH - RX_HEADER_LENGTH);                  \
} while (0)

//TODO finish comment
UARTservo::rx_result_t UARTservo::receiveSerial(void){
  STARTUP_ASSERT_RX();
  rx_status_t receive_status;
  rx_result_t result;

    do {
      // reading UART and checking whether there is any new data
      uint8_t character = 0;
      receive_status = receive_character(&character);

      //processing starts only if reading of the character has been succesfull
      if (receive_status == DATA_READ || receive_status == DATA_READ_MORE_AVAILABLE ) {

        /* state machine for processing data,
         * state 1 - waiting for the 2 keys
         * state 2 - putting data into the buffer and decoding them
        */
        if (_begin_receive == WAITING_FOR_KEYS) {
            // state 1 - waiting for 2 keys in a row
            wait_for_keys(character);
            result = RECEIVING;

        } else {
            // state 2 - putting data into the buffer and decoding them
            result = receive_data(character);

        }

      }
      else {
        result = NO_NEW_DATA;
      }
    } while (receive_status == DATA_READ_MORE_AVAILABLE);

  return result;

}

//TODO finish comment
void UARTservo::wait_for_keys(uint8_t received_character){
  // state 1 - waiting for 2 keys in a row
  if (received_character == KEY){
    _num_of_keys++;
    }

  else if (_num_of_keys >= REQ_NUM_OF_KEYS) {
    //put character containing message type into buffer and test if it is invalid
    _received_count = 2;
    _rx_receive_space.buffer[_received_count]=received_character;

    //testing validity
    if ( _rx_receive_space.message.header.message_type <= RX_DEBUG)
      {
        //assigning the first 2 keys only because of the checksum;
        _rx_receive_space.message.header.key[0]=KEY;
        _rx_receive_space.message.header.key[1]=KEY;

        //starting receiving of the other characters
        _begin_receive = RECEIVING_STARTED;
      }
      else
      {
        //reset receiving
        _begin_receive = WAITING_FOR_KEYS;
        _received_count = 0;

      }
      _num_of_keys = 0;
    }


}

//TODO finish comment
UARTservo::rx_result_t UARTservo::receive_data(uint8_t received_character){
  STARTUP_ASSERT_RX();
  // state 2 - putting data into the buffer and decoding them
  _received_count++;

  //we have to find which message we have received based on the msg type
  checksum_status_t checksum;

  switch (_rx_receive_space.message.header.message_type) {

  //feedback message
  case RX_FEEDBACK:
      checksum = receive_and_check(received_character, &_rx_receive_space.message.data.feedback.checksum, RX_FEEDBACK_LENGTH);
      if (checksum == WAITING_FOR_MORE_DATA){
        return RECEIVING;

      } else if (checksum == CHECKSUM_OK){
        //Checksum OK, read data
        read_rx_feedback();
        return NEW_MSG_FEEDBACK;

      } else if (checksum == ID_WRONG){
        return INCORRECT_ID;

      } else {
        return MSG_CORRUPTED;
      }
      break;

  //service data message
  case RX_SERVICE_DATA:
      checksum = receive_and_check(received_character, &_rx_receive_space.message.data.service_data.checksum, RX_SERVICE_DATA_LENGTH);
      if (checksum == WAITING_FOR_MORE_DATA){
        return RECEIVING;

      } else if (checksum == CHECKSUM_OK){
        //Checksum OK, read data
        read_rx_service_data();
        return NEW_MSG_SERVICE_DATA;

      } else if (checksum == ID_WRONG){
        return INCORRECT_ID;

      } else {
        return MSG_CORRUPTED;
      }
      break;

  //settings message
  case RX_SETTINGS:
      checksum = receive_and_check(received_character, &_rx_receive_space.message.data.settings.checksum, RX_SETTINGS_LENGTH);
      if (checksum == WAITING_FOR_MORE_DATA){
        return RECEIVING;

      } else if (checksum == CHECKSUM_OK){
        //Checksum OK, read data
        read_rx_settings();
        return NEW_MSG_SETTINGS;

      } else if (checksum == ID_WRONG){
        return INCORRECT_ID;

      } else {
        return MSG_CORRUPTED;
      }
      break;

  //errors message
  case RX_ERRORS:
      checksum = receive_and_check(received_character, &_rx_receive_space.message.data.errors.checksum, RX_ERRORS_LENGTH);
      if (checksum == WAITING_FOR_MORE_DATA){
        return RECEIVING;

      } else if (checksum == CHECKSUM_OK){
        //Checksum OK, read data
        read_rx_errors();
        return NEW_MSG_ERRORS;

      } else if (checksum == ID_WRONG){
        return INCORRECT_ID;

      } else {
        return MSG_CORRUPTED;
      }
      break;

  //errors message
  case RX_DEBUG:
      checksum = receive_and_check(received_character, &_rx_receive_space.message.data.debug.checksum, RX_DEBUG_LENGTH);
      if (checksum == WAITING_FOR_MORE_DATA){
        return RECEIVING;
      } else if (checksum == CHECKSUM_OK){
        //Checksum OK, read data
        read_rx_debug();
        return NEW_MSG_DEBUG;

      } else if (checksum == ID_WRONG){
        return INCORRECT_ID;

      } else {
        return MSG_CORRUPTED;
      }
      break;

  default:
      //TODO add handling of incorrect message name
      _begin_receive = WAITING_FOR_KEYS;
      _received_count = 0;
      return MSG_CORRUPTED;
  break;

  }

}

//TODO finish comment
UARTservo::checksum_status_t UARTservo::receive_and_check(uint8_t received_character,
		uint16_t *checksum, RXMessageLengthTypeDef messageLength) {

	_rx_receive_space.buffer[_received_count] = received_character;
	//receive characters until the message is fully received, then decode it
	if (_received_count == messageLength - 1) {
		//after receiving the correct amount of data, reset receiving
		_begin_receive = WAITING_FOR_KEYS;
		_received_count = 0;

    if (_rx_receive_space.message.header.servo_ID != _servo_id){
      return ID_WRONG;
    }
	   if (*checksum ==  calc_checksum(_rx_receive_space.buffer, messageLength)) {
		     return CHECKSUM_OK;
	      }
	  return CHECKSUM_WRONG;
	}
	return WAITING_FOR_MORE_DATA;
}

//TODO finish comment
void UARTservo::read_rx_header(){
  header.setContent(
     _rx_receive_space.message.header.motor_state,
     _rx_receive_space.message.header.warnings,
     _rx_receive_space.message.header.errors,
     _rx_receive_space.message.header.permanent_errors);
}

//TODO finish comment
void UARTservo::read_rx_feedback(){
  read_rx_header();
  feedback.setContent(
     _rx_receive_space.message.data.feedback.position,
     _rx_receive_space.message.data.feedback.pos_error,
     _rx_receive_space.message.data.feedback.torque,
     _rx_receive_space.message.data.feedback.speed,
     _rx_receive_space.message.data.feedback.temperature);
}

//TODO finish comment
void UARTservo::read_rx_service_data(){
  read_rx_header();
  service_data.setContent(
     _rx_receive_space.message.data.service_data.EL_serial_number,
     _rx_receive_space.message.data.service_data.User_String,
     _rx_receive_space.message.data.service_data.ME_serial_number,
     _rx_receive_space.message.data.service_data.run_time,
     _rx_receive_space.message.data.service_data.sw_version,
     _rx_receive_space.message.data.service_data.servo_wear);
}

//TODO finish comment
void UARTservo::read_rx_settings(){
  read_rx_header();
  settings.setContent(
     _rx_receive_space.message.data.settings.P_setting,
     _rx_receive_space.message.data.settings.I_setting,
     _rx_receive_space.message.data.settings.D_setting,

     _rx_receive_space.message.data.settings.zero_position,
     _rx_receive_space.message.data.settings.min_position,
     _rx_receive_space.message.data.settings.max_position,
     _rx_receive_space.message.data.settings.CurLim_setting,
     _rx_receive_space.message.data.settings.VoltLim_setting,

     _rx_receive_space.message.data.settings.servo_ID,
     _rx_receive_space.message.data.settings.group_ID,
     _rx_receive_space.message.data.settings.servo_mode,

     _rx_receive_space.message.data.settings.feedback_interval,
     _rx_receive_space.message.data.settings.data_interval,
     _rx_receive_space.message.data.settings.settings_interval,
     _rx_receive_space.message.data.settings.errors_interval,
     _rx_receive_space.message.data.settings.data_interval);
}

//TODO finish comment
void UARTservo::read_rx_errors(){
  read_rx_header();
  errors.setContent(
    _rx_receive_space.message.data.errors.warnings,
    _rx_receive_space.message.data.errors.errors,
    _rx_receive_space.message.data.errors.permanent_errors,
    _rx_receive_space.message.data.errors.service_intervals,
    _rx_receive_space.message.data.errors.eeprom_errors,
    _rx_receive_space.message.data.errors.uart_errors);
}

//TODO finish comment
void UARTservo::read_rx_debug(){
  read_rx_header();
  debug.setContent(
     _rx_receive_space.message.data.debug.raw_position,
     _rx_receive_space.message.data.debug.input_voltage,
     _rx_receive_space.message.data.debug.output_voltage,
     _rx_receive_space.message.data.debug.motor_voltage,
     _rx_receive_space.message.data.debug.phase_current,
     _rx_receive_space.message.data.debug.output_power,
     _rx_receive_space.message.data.debug.current_consumption,
     _rx_receive_space.message.data.debug.power_consumption,
     _rx_receive_space.message.data.debug.consumed_current,
     _rx_receive_space.message.data.debug.consumed_power);
}
