#include "airplane_flight_control.hpp"

#include "autopilotconf.h"
#include "ch.h"
#include "hal.h"

void AirplaneFlightControl::main()
{

  while(1)
  {
    readRadioInputs();
    readSensorInputs();

    RollPitchYaw rpyAngularRates = radioToAngularRate();

    Quaternion<float> desiredOrientation = radioToOrientation();

    float desiredAirSpeed = radioToDesiredSpeed();
    throttleArm();

    writeOutputs(rpyAngularRates, desiredOrientation, desiredAirSpeed);
    chThdSleepMicroseconds(1000);
  }
}

Quaternion<float> AirplaneFlightControl::radioToOrientation(){

  Quaternion<float> desiredOrientation;
  return desiredOrientation;
}

RollPitchYaw AirplaneFlightControl::radioToAngularRate(){
  RollPitchYaw rpyAngularRates;

  rpyAngularRates.roll = axisToAngularRate(_in_sbus.roll, ROLL_ANGULAR_GAIN);
  rpyAngularRates.pitch = axisToAngularRate(_in_sbus.pitch, PITCH_ANGULAR_GAIN);
  rpyAngularRates.yaw = axisToAngularRate(_in_sbus.yaw, YAW_ANGULAR_GAIN);

  return rpyAngularRates;
}

float AirplaneFlightControl::radioToDesiredSpeed(){
  float desiredAirSpeed;
    desiredAirSpeed = ((_in_sbus.throttle+1)/2)*(MAX_AIRSPEED-MIN_AIRSPEED)+MIN_AIRSPEED;

  return desiredAirSpeed;
}

void AirplaneFlightControl::throttleArm(){
  //the airplane will be armed only if throttle is not HIGH,
  if ((_in_armed == TRUE && _signal_available == TRUE && _armed == FALSE && _in_sbus.throttle < -0.95)
  || (_in_armed == TRUE && _armed == TRUE)){
    _armed = 1;
  }
  else {
    _armed = 0;
  }
}

float AirplaneFlightControl::axisToAngularRate(float axis, float gain){
  return axis*gain;
}
