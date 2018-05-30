#pragma once
#include <ch.hpp>

#include "sbus_reader.hpp"
#include "airplane_orientation_control.hpp"

#include "quaternion.h"

//TODO: implement more elegant way of PID tuning.
struct PIDtune {
  float kP;
  float kI;
  float kDff;
};

class AirplaneFlightControl : public chibios_rt::BaseStaticThread<2048> {
private:
  void readRadioInputs();
  void readSensorInputs();

  float axisToAngularRate(float axis, float gain);
  RollPitchYaw radioToAngularRate();
  Quaternion<float> radioToOrientation();
  float radioToDesiredSpeed();

  void throttleArm();
  void writeOutputs(RollPitchYaw desiredAngularRates, Quaternion<float> desiredOrientation, float desiredAirSpeed);

  //Input variables
  bool _signal_available;
  bool _in_armed;
  bool _in_airspeed_control;
  bool _in_stabilisation;
  bool _in_heading_control;

  SbusChannnels _in_sbus;

  //sensor variables
  Quaternion<float> _in_orientation;
  float _batt_voltage;
  float _air_speed;

  //running variables
   bool _armed;

  //time stamps
   uint32_t _timestampInSbus;
   uint32_t _timestampInOrientation;
   uint32_t _timestampBattVoltage;
   uint32_t _timestampAirSpeed;

public:
  AirplaneFlightControl() :
                  _signal_available(false),
                  _in_armed(false),
                  _in_airspeed_control(false),
                  _in_stabilisation(false),
                  _in_heading_control(false),
                  _in_sbus(),
                  _in_orientation(),

                  _batt_voltage(0),
                  _air_speed(0),
                  _armed(false),

                  _timestampInSbus(0),
                  _timestampInOrientation(0),
                  _timestampBattVoltage(0),
                  _timestampAirSpeed(0) {}

    void main();
  /* data */
};
