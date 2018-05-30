#pragma once
#include <ch.hpp>
#include "vector.h"

//can be used for storing angular rates, or raw values
struct RollPitchYaw {
  float roll;
  float pitch;
  float yaw;
};

class AirplaneMixer : public chibios_rt::BaseStaticThread<512> {
private:
  void writeOutputs();
  void readInputs();

  float _throttle;
  RollPitchYaw _rollPitchYaw;

  uint32_t _timestampThrottle;
  uint32_t _timestampRollPitchYaw;

public:
  AirplaneMixer() :
               _throttle(),
               _rollPitchYaw(),

               _timestampThrottle(0),
               _timestampRollPitchYaw(0) {}
    void main();
  /* data */
};
