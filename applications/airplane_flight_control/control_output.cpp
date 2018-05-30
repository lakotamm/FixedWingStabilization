#include "airplane_flight_control.hpp"

#include "ch.h"
#include "hal.h"

#include "airplane_orientation_control.hpp"
#include "app_wrapper.hpp"


void AirplaneFlightControl::writeOutputs(RollPitchYaw desiredAngularRates, Quaternion<float> desiredOrientation, float desiredAirSpeed){
  //writing data for the orientation control
  ControlOrientation controlOrientation;

  controlOrientation.rawRPY.roll = _in_sbus.roll/2;
  controlOrientation.rawRPY.pitch = _in_sbus.pitch/2;
  controlOrientation.rawRPY.yaw = _in_sbus.yaw/2;
  controlOrientation.desiredAngularRate =  desiredAngularRates;
  controlOrientation.desiredOrientation = desiredOrientation;
  controlOrientation.mixAngleOrientation = 0;

  if (_in_heading_control || _in_stabilisation) {
    controlOrientation.bypassOrientation = false;
  }
  else {
    controlOrientation.bypassOrientation = true;
  }

  sharedData::autopilot::controlOrientation.write(controlOrientation, apps::timeUS());

  //writing data for the speed/throttle control
  ControlSpeed controlSpeed;

  controlSpeed.armed =_armed;

  if(_in_airspeed_control){
   controlSpeed.bypassThrottle = false;
  } else {
    controlSpeed.bypassThrottle = true;
  }
  controlSpeed.desiredSpeed = desiredAirSpeed;
  controlSpeed.throttle = (_in_sbus.throttle+1)/2; //0 to full power ignoring reverse

  sharedData::autopilot::controlSpeed.write(controlSpeed, apps::timeUS());

  //output to PID tuning
  PIDtune pidTune;
  pidTune.kP =(_in_sbus.yaw + 1)/2;
  pidTune.kI =(_in_sbus.channel7 + 1)/2;
  pidTune.kDff =(_in_sbus.channel8 + 1)/2;

  sharedData::autopilot::pidTune.write(pidTune,apps::timeUS());

}
