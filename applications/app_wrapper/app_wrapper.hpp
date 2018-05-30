#pragma once
/* This file has been heavily censured and therefore it will not compile.*/

#include "servo_control.hpp"
#include "sbus_reader.hpp"
#include "analog_reader.hpp"

#include "airplane_flight_control.hpp"
#include "airplane_orientation_control.hpp"
#include "airplane_speed_control.hpp"
#include "airplane_mixer.hpp"

namespace threads {
// #if (ENABLE_THREADS)
#if (THREAD_FUSION_ESTIMATOR)
  extern OrientationFusionThread orientationFusionThread;
#endif

#if (THREAD_BMI160_READER)
  extern ImuReaderThread imuReaderThread;
#endif

#if (THREAD_BME280_READER)
  extern Bme280ReaderThread bme280ReaderThread;
#endif

#if (THREAD_SENSOR_TESTER)
  extern SensorTesterThread sensorTesterThread;
#endif

#if (THREAD_SEQUENTIAL_PRINTER)
  extern SequentialPrinterThread sequentialPrinterThread;
#endif

#if (THREAD_GPS_READER)
  extern GpsReaderThread gpsReaderThread;
#endif

#if (THREAD_CONTROLLER)
  extern ControllerThread controllerThread;
#endif

#if (THREAD_POSITION_ESTIMATOR)
  extern PositionEstimatorThread positionEstimatorThread;
#endif

#if (THREAD_WATCHDOG)
  extern Watchdog watchdog;
#endif

#if (THREAD_HEIGHT_ESTIMATOR_V2)
  extern HeightEstimatorV2 heightEstimatorV2;
#endif

#if (THREAD_HEIGHT_ESTIMATOR_V3)
  extern HeightEstimatorV3 heightEstimatorV3;
#endif

#if (THREAD_AIRPLANE_FLIGHT_CONTROL)
  extern AirplaneFlightControl airplaneFlightControl;
#endif

#if (THREAD_AIRPLANE_ORIENTATION_CONTROL)
  extern AirplaneOrientationControl airplaneOrientationControl;
#endif

#if (THREAD_AIRPLANE_SPEED_CONTROL)
  extern AirplaneSpeedControl airplaneSpeedControl;
#endif

#if (THREAD_AIRPLANE_MIXER)
  extern AirplaneMixer airplaneMixer;
#endif

#if (THREAD_SERVO_CONTROL)
  extern ServoControl servoControl;
#endif

#if (THREAD_SBUS_READER)
  extern SbusReader sbusReader;
#endif

#if (THREAD_ANALOG_READER)
  extern AnalogReader analogReader;
#endif
}

namespace sharedData {
  namespace meas {
    extern Measurement<Vector<float>> acc;
    extern Measurement<Vector<float>> gyr;
    extern Measurement<Vector<float>> mag;

    extern DoubleBuffer<float> airSpeed;
    extern DoubleBuffer<float> diffPressure; //differential pressure from the pitot tube in Pa
    extern DoubleBuffer<float> battVoltage;  //battery voltage in Volts
  }
  namespace esti {
    extern DoubleBuffer<Quaternion<float>> q; // Board orientation with respect to the Earth frame i.e q = BqE : vB = BqE * (0, vE) * BqE^*

  }
  namespace autopilot{
    //input form the receiver
    extern DoubleBuffer<SbusChannnels> sbusChannnels;

    //output to Airplane orientation control
    extern DoubleBuffer<ControlOrientation> controlOrientation;


    //output to Airplane speed control
    extern DoubleBuffer<ControlSpeed> controlSpeed;

    //output to Mixer
    extern DoubleBuffer<float> outThrottle;
    extern DoubleBuffer<RollPitchYaw> outRollPitchYaw;

    //output to servo control
    extern DoubleBuffer<ControlServos> controlServos;

    //PIDtune out from the flight control
    extern DoubleBuffer<PIDtune> pidTune;

  }

}
