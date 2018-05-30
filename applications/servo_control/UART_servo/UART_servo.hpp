#include "Porting_layer.hpp"

#include "MASTER_UART_RX_msg.h"
#include "MASTER_UART_TX_msg.h"

#include "RX_space_and_functions.hpp"

#define KEY 0xff

class UARTservo  {

  public:
    /*
    * @brief Initialise servo on serial port
    * @param serial: Pointer to the instance of the serial port
    */
    UARTservo(uint8_t servo_id, SerialDriver* serial) :
          _servo_id(servo_id),
          _servo_serial(serial),
          _rx_receive_space(),
          _tx_transmit_space(),
          _num_of_keys(0),
          _received_count(0),
          _begin_receive(WAITING_FOR_KEYS),
          header(),
          feedback(),
          service_data(),
          settings(),
          errors(),
          debug() {}

    void Init(void);
    void DeInit(void);

    enum tx_result_t {
      SERIAL_DATA_SENT,
      SERIAL_BUSY,
      PARAM_OUT_OF_RANGE
    };

    enum rx_result_t {
      NO_NEW_DATA,
      RECEIVING,
      MSG_CORRUPTED,
      INCORRECT_ID,
      NEW_MSG_FEEDBACK,
      NEW_MSG_SERVICE_DATA,
      NEW_MSG_SETTINGS,
      NEW_MSG_ERRORS,
      NEW_MSG_DEBUG,
    };

    //Send data to the servo
    tx_result_t setAbsPosition(int16_t position);
    tx_result_t setAbsPosition(int16_t position, bool groupControl);
    tx_result_t setRelPosition(int16_t position);
    tx_result_t setRelPosition(int16_t position, bool groupControl);

    tx_result_t setZeroPosition(int16_t position);
    tx_result_t setMinPosition(int16_t position);
    tx_result_t setMaxPosition(int16_t position);

    tx_result_t setMotor(bool motor);
    tx_result_t setMotor(bool motor, bool groupControl);

    tx_result_t setSettings(uint16_t pSetting, uint16_t iSetting,
      uint16_t dSetting, uint16_t currentLimit, uint16_t voltageLimit,
      uint8_t servo_ID, uint8_t group_ID, uint8_t servo_mode);
    tx_result_t setUsrString(uint8_t *userString, uint8_t length);

    tx_result_t enterBootloader(void);
    tx_result_t resetErrors(void);
    tx_result_t factoryReset(void); 
    tx_result_t writeSettingsEEPROM(void);
    tx_result_t writeUsrStringEEPROM(void);

    tx_result_t requestFeedback(uint8_t interval);
    tx_result_t requestServiceData(uint8_t interval);
    tx_result_t requestSettings(uint8_t interval);
    tx_result_t requestErrors(uint8_t interval);
    tx_result_t requestDebug(uint8_t interval);

    //Receiving data from the servo
    rx_result_t receiveSerial(void);

    header_t getHeader(){
      return header;
    }

    feedback_t getFeedback(){
      return feedback;
    }

    service_data_t getService_data(){
      return service_data;
    }

    settings_t getSettings(){
      return settings;
    }

    errors_t getErrors(){
      return errors;
    }

    debug_t getDebug(){
      return debug;
    }

  private:
    enum receive_state_t {
      WAITING_FOR_KEYS,
      RECEIVING_STARTED
    };

    //variables
    uint8_t _servo_id;
    SerialDriver *_servo_serial;

    RX_receive_space_t _rx_receive_space;
    TX_transmit_space_t _tx_transmit_space;

    uint8_t _num_of_keys;
    uint8_t _received_count;

    receive_state_t _begin_receive;

    //received messages
    header_t header;
    feedback_t feedback;
    service_data_t service_data;
    settings_t settings;
    errors_t errors;
    debug_t debug;

    enum rx_status_t {
      NO_DATA_AVAILABLE,
      DATA_READ,
      DATA_READ_MORE_AVAILABLE
    };

    enum checksum_status_t {
      WAITING_FOR_MORE_DATA,
      CHECKSUM_OK,
      CHECKSUM_WRONG,
      ID_WRONG
    };


    //HW functions
    tx_result_t send_data(TXMessageLengthTypeDef messageLength);
    rx_status_t receive_character(uint8_t *character);
    uint16_t calc_checksum(uint8_t *buffer, uint8_t messageLength);

    //TX functions
    tx_result_t sendCommand(TXMessageIDTypeDef message_type, bool group_flag);
    tx_result_t sendRqstMsg(TXMessageIDTypeDef message_type, uint8_t interval, bool group_flag);
    tx_result_t sendPosition(TXMessageIDTypeDef message_type, int16_t position, bool group_flag);
    tx_result_t sendSettings(uint16_t pSetting, uint16_t iSetting,
      uint16_t dSetting, uint16_t currentLimit, uint16_t voltageLimit,
      uint8_t servo_ID, uint8_t group_ID, uint8_t servo_mode, bool group_flag);
    tx_result_t sendUsrString(uint8_t *userString, bool group_flag);

    void clean_buffer_tx(TXMessageLengthTypeDef messageLength);
    void assign_header_tx(TXMessageIDTypeDef message_type, bool group_flag);

    //RX functions
    void wait_for_keys(uint8_t received_character);
    rx_result_t receive_data(uint8_t received_character);
    checksum_status_t receive_and_check(uint8_t received_character, uint16_t *checksum, RXMessageLengthTypeDef messageLength);

    //rx copying received data
    void read_rx_header();
    void read_rx_feedback();
    void read_rx_service_data();
    void read_rx_settings();
    void read_rx_errors();
    void read_rx_debug();
};
