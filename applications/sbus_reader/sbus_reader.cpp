#include "ch.h"
#include "hal.h"

#include "sbus_reader.hpp"

#include "../airplane_speed_control/airplane_speed_control.hpp"
#include "sbus.h"
#include "debugger.hpp"
#include "app_wrapper.hpp"

#define PITCH_CHANNEL     1
#define ROLL_CHANNEL      0
#define YAW_CHANNEL       3
#define THROTTLE_CHANNEL  2

#define SBUS_CENTER    1023
#define SBUS_RANGE      940

void SbusReader::main()
{

  while(1)
  {
      if(readAndDecode()){
        SbusChannnels sbusWrite;
        sbusWrite.pitch = readPitch();
        sbusWrite.roll = readRoll();
        sbusWrite.yaw = readYaw();
        sbusWrite.throttle = readThrottle();
        sbusWrite.channel5 = readChannel(4);
        sbusWrite.channel6 = readChannel(5);
        sbusWrite.channel7 = readChannel(6);
        sbusWrite.channel8 = readChannel(7);

        sharedData::autopilot::sbusChannnels.write(sbusWrite, apps::timeUS());
     }
    chThdSleepMicroseconds(100);
  }
}

bool SbusReader::readAndDecode() {
  uint32_t bytes_available = chQSpaceI(&_sbus_serial.iqueue);
  //check whether there is anything to read
  if (bytes_available){
    uint8_t character;
    //sdAsynchronousRead(&_sbus_serial, &character, 1);
    sdRead(&_sbus_serial, &character, 1);

    return sbusDecode(character);
  }
  return 0;
}

float SbusReader::readChannel(uint8_t channel){
//  dprint("%1d",sbusGetChannel(PITCH_CHANNEL)-SBUS_CENTER);
  if(sbusGetChannel(channel)){
    return (float) (sbusGetChannel(channel)-SBUS_CENTER)/SBUS_RANGE;
  }
  else {
    return 0;
  }

}

float SbusReader::readPitch(){
  if(sbusGetChannel(PITCH_CHANNEL)){
    float pitch = (float) (sbusGetChannel(PITCH_CHANNEL)-SBUS_CENTER)/SBUS_RANGE;
    return pitch;
  }
  else {
    return 0;
  }

}

float SbusReader::readRoll(){
  if(sbusGetChannel(ROLL_CHANNEL)){
    float roll = (float) (sbusGetChannel(ROLL_CHANNEL)-SBUS_CENTER)/SBUS_RANGE;
    return roll;
  }
  else {
    return 0;
  }

}
float SbusReader::readYaw(){
  if(sbusGetChannel(YAW_CHANNEL)){
    float yaw = (float) (sbusGetChannel(YAW_CHANNEL)-SBUS_CENTER)/SBUS_RANGE;
    return yaw;
  }
  else {
    return 0;
  }

}

float SbusReader::readThrottle(){
  if(sbusGetChannel(THROTTLE_CHANNEL)){
    float throttle = (float) -(sbusGetChannel(THROTTLE_CHANNEL)-SBUS_CENTER)/SBUS_RANGE;
    return throttle;
  }
  else {
    return 0;
  }

}
