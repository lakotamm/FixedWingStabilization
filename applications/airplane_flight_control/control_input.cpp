#include "airplane_flight_control.hpp"
#include "ch.h"
#include "hal.h"

#include "debugger.hpp"
#include "app_wrapper.hpp"

/*
Input shall be a float from -1 to 1
*/
void AirplaneFlightControl::readRadioInputs(){
  _signal_available = 1; //TODO: add proper check

   sharedData::autopilot::sbusChannnels.read(_in_sbus, _timestampInSbus);

  //arming and stabilization
   if ((_in_sbus.channel5 >- 0.1)
     && (_in_sbus.channel5 < 0.1)){
     //manual throttle
     _in_armed = true;
     _in_airspeed_control = false;

   } else if (_in_sbus.channel5 > 0.1){
     //airspeed control active
     _in_armed = true;
     _in_airspeed_control = true;
   } else {
     //throttle disabled
     _in_armed = false;
     _in_airspeed_control = false;
   }

  //one channel for controlling switching in between 3 modes:
  //pass through, stabilization enabled, heading control enables
  if ((_in_sbus.channel6 >- 0.1)
    && (_in_sbus.channel6 < 0.1)){
    //Stabilization active
    _in_stabilisation = true;
    _in_heading_control = false;
  } else if (_in_sbus.channel6 > 0.1){
    //heading control active
    _in_stabilisation = false;
    _in_heading_control = true;
  } else {
    //bypass active
    _in_stabilisation = false;
    _in_heading_control = false;
  }

}

void AirplaneFlightControl::readSensorInputs(){

   sharedData::esti::q.read(_in_orientation, _timestampInOrientation);
   sharedData::meas::battVoltage.read(_batt_voltage, _timestampBattVoltage);
   sharedData::meas::airSpeed.read(_air_speed, _timestampAirSpeed);

}
