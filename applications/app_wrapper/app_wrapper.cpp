#include "app_wrapper.hpp"
/* This file has been heavily censured and therefore it will not compile.*/

namespace threads {
#if (THREAD_AIRPLANE_FLIGHT_CONTROL)
  AirplaneFlightControl airplaneFlightControl;
#endif

#if (THREAD_AIRPLANE_ORIENTATION_CONTROL)
  AirplaneOrientationControl airplaneOrientationControl;
#endif

#if (THREAD_AIRPLANE_SPEED_CONTROL)
  AirplaneSpeedControl airplaneSpeedControl;
#endif

#if (THREAD_AIRPLANE_MIXER)
  AirplaneMixer airplaneMixer;
#endif

#if (THREAD_SERVO_CONTROL)
  ServoControl servoControl;
#endif

#if (THREAD_SBUS_READER)
  SbusReader sbusReader(SD3);
#endif

#if (THREAD_ANALOG_READER)
  AnalogReader analogReader;
#endif
}

namespace sharedData {
  namespace meas {
    Measurement<Vector<float>> acc;
    Measurement<Vector<float>> gyr;
    Measurement<Vector<float>> mag;

    DoubleBuffer<float> airSpeed;
    DoubleBuffer<float> diffPressure;
    DoubleBuffer<float> battVoltage;
  }
  namespace esti {
    DoubleBuffer<Quaternion<float>> q;
 
  }
  namespace autopilot {
    DoubleBuffer<SbusChannnels> sbusChannnels;

    //output to Airplane orientation control
    DoubleBuffer<ControlOrientation> controlOrientation;

    //output to Airplane speed control
    DoubleBuffer<ControlSpeed> controlSpeed;

    //output to Mixer
    DoubleBuffer<float> outThrottle;
    DoubleBuffer<RollPitchYaw> outRollPitchYaw;

    //output to servo control
    DoubleBuffer<ControlServos> controlServos;

    //PIDtune out from the flight control
    DoubleBuffer<PIDtune> pidTune;
  }
}
