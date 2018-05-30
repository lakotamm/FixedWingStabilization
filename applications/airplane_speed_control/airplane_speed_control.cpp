#include "airplane_speed_control.hpp"
#include "ch.h"
#include "pid.hpp"
#include "hal.h"

#include "debugger.hpp"
#include "app_wrapper.hpp"
#include "autopilotconf.h"

void AirplaneSpeedControl::main()
{
    PIDSettings airspeedPIDSettings = {
                                   0.1,   //Kp
                                   0.1,   //Ki
                                   0,     //Kd
                                   0,     //Kff
                                   1,     //outer limits
                                   10,     //Dpart filter
                                   1      //accumulator limit
  };

  _airSpeedPID.init(airspeedPIDSettings);
  _airSpeedPID.enable();

  while(1)
  {
    readInputs();

    float outputThrottle = 0;
    outputThrottle =_airSpeedPID.spin(_airSpeed,_in_controlSpeed.desiredSpeed);

    writeOutputs(outputThrottle);
    chThdSleepMicroseconds(100);
  }
}

void AirplaneSpeedControl::readInputs(){
  sharedData::autopilot::controlSpeed.read(_in_controlSpeed, _timestampInControlSpeed);
  sharedData::meas::airSpeed.read(_airSpeed, _timestampAirSpeed);

  #if (TUNE_AIRSPEED_PID)
    PIDtune pidTune;
    uint32_t timestamp = 0;
    sharedData::autopilot::pidTune.read(pidTune,timestamp);
    float kp = pidTune.kP*5;
    float ki = pidTune.kI/5;
    float kd = pidTune.kDff;
    _airSpeedPID.setKp(kp);
    _airSpeedPID.setKi(ki);
    _airSpeedPID.setKd(kd);

    dprint("%s,%0.4f,%0.4f,%0.4f", "PID", kp, ki, kd);

    dprint("%s,%0.4f", "IAS", _airSpeed);
    dprint("%s,%0.4f", "DAS", _in_controlSpeed.desiredSpeed);
  #endif
}

void AirplaneSpeedControl::writeOutputs(float outputThrottle)
{
  if(_in_controlSpeed.armed){
    if(_in_controlSpeed.bypassThrottle){
      sharedData::autopilot::outThrottle.write(_in_controlSpeed.throttle, apps::timeUS());
      _airSpeedPID.reset();
    }
    else {
      sharedData::autopilot::outThrottle.write(outputThrottle, apps::timeUS());
    }
  }
  else {
    sharedData::autopilot::outThrottle.write(0, apps::timeUS());
  }
}
