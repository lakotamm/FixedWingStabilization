/*
 * angular_rate_control.cpp
 *
 *  Created on: May 9, 2018
 *      Author: das
 */
#include "airplane_orientation_control.hpp"

#include "pid.hpp"
#include "ch.h"
#include "hal.h"

void AirplaneOrientationControl::initAngularRatePIFFs(){
  PIDSettings pidSettingPitch = {
                                 10,    //Kp
                                 0.05,     //Ki
                                 0,     //Kd
                                 1,     //Kff
                                 1,     //outer limits
                                 0,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _angularRatePitchPIFF.init(pidSettingPitch);

  PIDSettings pidSettingRoll = {
                                 10,    //Kp
                                 0.05,     //Ki
                                 0,     //Kd
                                 1,     //Kff
                                 1,     //outer limits
                                 0,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _angularRateRollPIFF.init(pidSettingRoll);

  PIDSettings pidSettingYaw = {
                                 0,    //Kp
                                 0,     //Ki
                                 0,     //Kd
                                 1,     //Kff
                                 1,     //outer limits
                                 20,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _angularRateYawPIFF.init(pidSettingYaw);
  //disabled because we do not have yaw control
  //_angularRateYawPIFF.enable();


}
void AirplaneOrientationControl::enableAngularRatePIFFs(){
  _angularRatePitchPIFF.enable();
  _angularRateRollPIFF.enable();
  //disabled because we do not have yaw control
  //_angularRateYawPIFF.enable();
}

void AirplaneOrientationControl::disableAngularRatePIFFs(){
  _angularRatePitchPIFF.disable();
  _angularRateRollPIFF.disable();
  //disabled because we do not have yaw control
  //_angularRateYawPIFF.disable();
}

void AirplaneOrientationControl::resetAngularRatePIFFs(){
  _angularRatePitchPIFF.reset();
  _angularRateRollPIFF.reset();
  //disabled because we do not have yaw control
  //_angularRateYawPIFF.reset();
}

RollPitchYaw AirplaneOrientationControl::stabiliseAngularRate(
    RollPitchYaw desRPYrate, Vector<float> gyroscope, float attenuation) {
  //TODO: add proper PID stabilization
  RollPitchYaw outRollPitchYaw;
  outRollPitchYaw.pitch = _angularRatePitchPIFF.spin(gyroscope[1],desRPYrate.pitch,attenuation);
  outRollPitchYaw.roll = _angularRateRollPIFF.spin(gyroscope[0],desRPYrate.roll,attenuation);
  outRollPitchYaw.yaw = _angularRateYawPIFF.spin(gyroscope[2],desRPYrate.yaw,attenuation);

  return outRollPitchYaw;
}


