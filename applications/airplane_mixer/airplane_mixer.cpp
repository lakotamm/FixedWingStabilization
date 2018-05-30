#include "airplane_mixer.hpp"

#include "ch.h"
#include "hal.h"

#include "debugger.hpp"
#include "app_wrapper.hpp"

void AirplaneMixer::main()
{

  while(1)
  {
    readInputs();
    writeOutputs();
    chThdSleepMicroseconds(1000);
  }
}

void AirplaneMixer::readInputs(){
  sharedData::autopilot::outRollPitchYaw.read(_rollPitchYaw,_timestampRollPitchYaw);
  sharedData::autopilot::outThrottle.read(_throttle,_timestampThrottle);
}

void AirplaneMixer::writeOutputs()
  {
  ControlServos controlServos;

  controlServos.rightWingServo  =  (+_rollPitchYaw.pitch + _rollPitchYaw.roll) *800;
  controlServos.leftWingServo   =  (-_rollPitchYaw.pitch + _rollPitchYaw.roll) *800;

  //dprint("r,%1d", controlServos.rightWingServo);
  //dprint("l,%1d", controlServos.rightWingServo);
  controlServos.ESC = ((_throttle)*1000);

  sharedData::autopilot::controlServos.write(controlServos, apps::timeUS());
}
