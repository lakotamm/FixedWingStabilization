#pragma once
#include <ch.hpp>

struct ControlServos {
  int16_t ESC;
  int16_t leftWingServo;
  int16_t rightWingServo;
};

class ServoControl : public chibios_rt::BaseStaticThread<2048> {
private:
  void setThrottle();

  ControlServos _inControlServos;
  uint32_t _timestampInControlServos;

public:
  ServoControl() :
               _inControlServos(),

               _timestampInControlServos(0) {}
  void main();
  void readInputs();
  /* data */
};
