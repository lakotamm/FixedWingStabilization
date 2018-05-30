/*
 * orientation_control.cpp
 *
 *  Created on: May 9, 2018
 *      Author: das
 */
#include "airplane_orientation_control.hpp"

#include "pid.hpp"
#include "ch.h"
#include "hal.h"

void AirplaneOrientationControl::initOrientationPIDs(){

  PIDSettings pidSettingPitch = {
                                 20,    //Kp
                                 1,     //Ki
                                 1,     //Kd
                                 0,     //Kff
                                 1,     //outer limits
                                 20,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _orientationPitchPIFF.init(pidSettingPitch);

  PIDSettings pidSettingRoll = {
                                 20,    //Kp
                                 1,     //Ki
                                 1,     //Kd
                                 0,     //Kff
                                 1,     //outer limits
                                 20,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _orientationRollPIFF.init(pidSettingRoll);


  PIDSettings pidSettingYaw = {
                                 20,    //Kp
                                 1,     //Ki
                                 1,     //Kd
                                 0,     //Kff
                                 1,     //outer limits
                                 20,    //Dpart filter
                                 0.5    //accumulator limit
  };
  _orientationYawPIFF.init(pidSettingYaw);
}

void AirplaneOrientationControl::enableOrientationPIDs(){
  _orientationPitchPIFF.enable();
  _orientationRollPIFF.enable();
  //disabled because we do not have yaw control
  //_orientationYawPIFF.enable();
}

void AirplaneOrientationControl::disableOrientationPIDs(){
  _orientationPitchPIFF.disable();
  _orientationRollPIFF.disable();
  //disabled because we do not have yaw control
  //_orientationYawPIFF.disable();
}

void AirplaneOrientationControl::resetOrientationPIDs(){
  _orientationPitchPIFF.reset();
  _orientationRollPIFF.reset();
  //disabled because we do not have yaw control
  //_orientationYawPIFF.reset();
}

RollPitchYaw AirplaneOrientationControl::orientationToAngularRate(
    Quaternion<float> desiredOrientation, Quaternion<float> orientation) {

  RollPitchYaw orientationRateRPY;

  return orientationRateRPY;
}



