/*
 * input_output.cpp
 *
 *  Created on: May 7, 2018
 *      Author: das
 */
#include "airplane_orientation_control.hpp"
#include "app_wrapper.hpp"

void AirplaneOrientationControl::readInputs(){
  sharedData::autopilot::controlOrientation.read(_controlOrientation, _timestampControlOrientation);
  sharedData::meas::gyr.calibrated(_gyroscope,_timestampGyroscope);
  sharedData::esti::q.read(_orientation,_timestampOrientation);
  sharedData::meas::diffPressure.read(_diffPressure,_timestampDiffPressure);
  sharedData::meas::airSpeed.read(_airSpeed,_timestampAirspeed);

}

void AirplaneOrientationControl::writeOutputs(RollPitchYaw outputRPY){
  RollPitchYaw rollPitchYaw;

  if(_controlOrientation.bypassOrientation){
    rollPitchYaw =_controlOrientation.rawRPY;
  }
  else {
    rollPitchYaw = outputRPY;
  }

  sharedData::autopilot::outRollPitchYaw.write(rollPitchYaw, apps::timeUS());
}



