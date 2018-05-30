#include "servo_control.hpp"
#include "ch.h"
#include "hal.h"

#include "UART_servo/UART_servo.hpp"

#include "app_wrapper.hpp"
#include "debugger.hpp"
#include "enable_disable.hpp"

UARTservo right_servo(0,&SD7);
UARTservo left_servo(1,&SD7);


void ServoControl::main()
{

  while(1)
  {
    readInputs();
    #if (OUTPUT_UART_SERVO)
    right_servo.setAbsPosition(_inControlServos.rightWingServo);
    left_servo.setAbsPosition(_inControlServos.leftWingServo);
    #endif
      setThrottle();
      chThdSleepMilliseconds(2);
  }
}

void ServoControl::readInputs(){

  //TODO: add check of the input
  sharedData::autopilot::controlServos.read(_inControlServos, _timestampInControlServos);

}

void ServoControl::setThrottle(){
  //dprint("%1d",throttle/2 + 1500);
  #if (OUTPUT_PWM_ESC)
    pwmEnableChannel(&PWMD1, 0, (_inControlServos.ESC/2) + 1500);
  #endif

}
