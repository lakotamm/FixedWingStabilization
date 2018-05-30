#include "Porting_layer.hpp"
#include "UART_servo.hpp"

//define polynome for CRC
#define CRC16 0x8005
// Serial config for usb->serial
static const SerialConfig uartConfig =
{
  115200, // bit rate
  0,
  0,
  0
};

//TODO finish comment
void UARTservo::Init(void){
  // Initialize UART->serial
  sdStart(_servo_serial, &uartConfig);
}


//TODO finish comment
UARTservo::tx_result_t UARTservo::send_data(TXMessageLengthTypeDef messageLength){
//TODO: add MUTEX to avoid corruption
  uint32_t space_available = chQSpaceI(&(_servo_serial)->oqueue);
  //check whether there is enough space in the buffer
  if (space_available>messageLength){
    sdAsynchronousWrite(_servo_serial,_tx_transmit_space.buffer, messageLength);
    return SERIAL_DATA_SENT;
  }
  else
    return SERIAL_BUSY;
}

//TODO finish comment
UARTservo::rx_status_t UARTservo::receive_character(uint8_t *character){

  uint32_t bytes_available = chQSpaceI(&(_servo_serial)->iqueue);
  //check whether there is anything to read
  if (bytes_available){
    sdAsynchronousRead(_servo_serial, character, 1);
    if (bytes_available > 1)
      return DATA_READ_MORE_AVAILABLE;
    else
      return DATA_READ;
  }
  else
    return NO_DATA_AVAILABLE;

}

//TODO finish comment
uint16_t UARTservo::calc_checksum(uint8_t *buffer, uint8_t messageLength) {

  uint16_t out = 0;
    int bits_read = 0, bit_flag;

    messageLength--;
    messageLength--;

    while(messageLength > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*buffer >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            buffer++;
            messageLength--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

  return crc;
}
