#pragma once
#include <ch.hpp>

#include "quaternion.h"
#include "vector.h"
#include "airplane_mixer.hpp"

#include "pid.hpp"

struct ControlOrientation {
  RollPitchYaw rawRPY;
  RollPitchYaw desiredAngularRate;
  Quaternion<float> desiredOrientation;
  float mixAngleOrientation;
  bool bypassOrientation;

  ControlOrientation():
                  rawRPY(),
                  desiredAngularRate(),
                  desiredOrientation (),
                  mixAngleOrientation(0),
                  bypassOrientation(0) {}
};

class AirplaneOrientationControl : public chibios_rt::BaseStaticThread<2048> {
private:
  void readInputs();
  void writeOutputs(RollPitchYaw rollPitchYaw);

  RollPitchYaw orientationToAngularRate(Quaternion<float> desiredOrientation,
                                        Quaternion<float> orientation);

  RollPitchYaw orientationRateMixer(RollPitchYaw orientationRPY,
                                    RollPitchYaw angularRateRPY,
                                    float mixAngleOrientation);

  float calculateAtenuation(float pressure);

  RollPitchYaw stabiliseAngularRate(RollPitchYaw desRPYrate,
                                    Vector<float> gyroscope, float attenuation);
  void initAngularRatePIFFs();
  void enableAngularRatePIFFs();
  void disableAngularRatePIFFs();
  void resetAngularRatePIFFs();

  void initOrientationPIDs();
  void enableOrientationPIDs();
  void disableOrientationPIDs();
  void resetOrientationPIDs();

  //Inputs
  ControlOrientation _controlOrientation;
  Vector<float> _gyroscope;
  Quaternion<float> _orientation;
  float _diffPressure;
  float _airSpeed;

  //PIFF loops
  PID _orientationRollPIFF;
  PID _orientationPitchPIFF;
  PID _orientationYawPIFF;

  PID _angularRateRollPIFF;
  PID _angularRatePitchPIFF;
  PID _angularRateYawPIFF;

  //time stamps
  uint32_t _timestampControlOrientation;
  uint32_t _timestampGyroscope;
  uint32_t _timestampOrientation;
  uint32_t _timestampDiffPressure;
  uint32_t _timestampAirspeed;

public:
  AirplaneOrientationControl() :
                  _controlOrientation(),
                  _gyroscope(),
                  _orientation(),
                  _diffPressure(0),
                  _airSpeed(0),

                  _orientationRollPIFF(),
                  _orientationPitchPIFF(),
                  _orientationYawPIFF(),

                  _angularRateRollPIFF(),
                  _angularRatePitchPIFF(),
                  _angularRateYawPIFF(),

                  _timestampControlOrientation(0),
                  _timestampGyroscope(0),
                  _timestampOrientation(0),
                  _timestampDiffPressure(0),
                  _timestampAirspeed(0) {}

    void main();
  /* data */
};
