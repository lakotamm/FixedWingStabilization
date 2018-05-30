#include "airplane_orientation_control.hpp"

#include "pid.hpp"
#include "ch.h"
#include "hal.h"

#include "debugger.hpp"

void AirplaneOrientationControl::main() {

 initAngularRatePIFFs();
 initOrientationPIDs();

 enableAngularRatePIFFs();
 enableOrientationPIDs();

  while (1) {
    readInputs();

    float pidAttenuation = calculateAtenuation(_diffPressure);

    RollPitchYaw orientationRateRPY;

    orientationRateRPY = orientationToAngularRate(
        _controlOrientation.desiredOrientation, _orientation);

    RollPitchYaw desiredRPYrate;

    desiredRPYrate = orientationRateMixer(
        orientationRateRPY, _controlOrientation.desiredAngularRate,
        _controlOrientation.mixAngleOrientation);

    RollPitchYaw outRPY;
    outRPY = stabiliseAngularRate(desiredRPYrate, _gyroscope, pidAttenuation);

    //if we are bypassing the PIDs, we have to reset all the internal variables
    if(_controlOrientation.bypassOrientation){
      resetAngularRatePIFFs();
      resetOrientationPIDs();
    }

    writeOutputs(outRPY);
    chThdSleepMicroseconds(1000);
  }
}

RollPitchYaw AirplaneOrientationControl::orientationRateMixer(
    RollPitchYaw orientationRateRPY, RollPitchYaw angularRateRPY,
    float mixAngleOrientation) {
  //TODO: add proper mixing
  return angularRateRPY;
}

float AirplaneOrientationControl::calculateAtenuation(float diffPressure) {
  //TODO: add proper attenuation calculation
  float attenuation = 1;
  return attenuation;
}
