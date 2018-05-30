#pragma once
#include <ch.hpp>

#include "pid.hpp"

struct ControlSpeed {
  float desiredSpeed;
  float throttle;
  bool bypassThrottle;
  bool armed;
};

class AirplaneSpeedControl : public chibios_rt::BaseStaticThread<2048> {
private:
  void readInputs();
  void writeOutputs(float outputThrottle);

  ControlSpeed _in_controlSpeed;

  //PID loop for speed
  PID _airSpeedPID;
  float _airSpeed;

  uint32_t _timestampInControlSpeed;
  uint32_t _timestampAirSpeed;

public:
  AirplaneSpeedControl() :
               _in_controlSpeed(),
               _airSpeedPID(),
               _airSpeed(0),
               _timestampInControlSpeed(0),
               _timestampAirSpeed(0) {}
    void main();
  /* data */
};
