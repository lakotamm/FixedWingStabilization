#pragma once
#include <ch.hpp>
#include "hal.h"

struct SbusChannnels {
  float roll;
  float pitch;
  float throttle;
  float yaw;
  float channel5;
  float channel6;
  float channel7;
  float channel8;
};

class SbusReader : public chibios_rt::BaseStaticThread<2048> {
private:
  /* data */
  SerialDriver & _sbus_serial;

  bool readAndDecode();
  float readChannel(uint8_t channel);
  float readPitch();
  float readRoll();
  float readYaw();
  float readThrottle();

public:
    SbusReader(SerialDriver & sbus_serial) : _sbus_serial(sbus_serial) {}

    void main();
  /* data */
};
