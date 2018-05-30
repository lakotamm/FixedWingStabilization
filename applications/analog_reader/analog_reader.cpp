#include "analog_reader.hpp"
#include "ch.h"
#include "hal.h"

#include "debugger.hpp"
#include "app_wrapper.hpp"


#define BUFFER_LENGTH 2    // number of used ADC channels
static adcsample_t samples_buf[BUFFER_LENGTH]; // ADC array

// Fill ADCConversionGroup structure fields
static ADCConversionGroup adccg = {

      // set to TRUE if need circular buffer, set FALSE otherwise
      true,

      // number of channels
      BUFFER_LENGTH,
      // sample done callback
      NULL,

      // ADC Error callback
      NULL,

      // CR1
      0,
      // CR2
      ADC_CR2_SWSTART,// |
      //ADC_CR2_CONT,

      // SMRP1 register content, set to zero for begin
      0,
      // SMRP2 register content, set to zero for begin
      ADC_SMPR2_SMP_AN5(ADC_SAMPLE_480) |
      ADC_SMPR2_SMP_AN6(ADC_SAMPLE_480),
      // SQR1 register content. Set channel sequence length
      ADC_SQR1_NUM_CH(BUFFER_LENGTH),
      // SQR2 register content, set to zero for begin
      0,
      // SQR3 register content.
      ADC_SQR3_SQ1_N(ADC_CHANNEL_IN6) |
      ADC_SQR3_SQ2_N(ADC_CHANNEL_IN5)
};

void AnalogReader::main()
{

  adcStartConversion(&ADCD1, &adccg, samples_buf, 2);
  while(1)
  {

    float pressure = diffPressureRaw;
    pressure = pressure + ((((float) samples_buf[0])-pressure)/20);
    diffPressureRaw = pressure;

    float voltage = battVoltageRaw;
    voltage = voltage + ((((float) samples_buf[1])-voltage)/20);
    battVoltageRaw = voltage;

    float battVoltage = battVoltageRaw/VOLTAGE_COEF;

    //equation assumes that the sensor is linear and the center is in the middle
    float diffPressure = -((BAROMETER_GAIN*diffPressureRaw)/BAROMETER_CENTER - BAROMETER_GAIN)*1000;

    float airSpeed;

    //airspeed is based on pressure difference and air density
    if (diffPressure >= 0){
      airSpeed = sqrt(2*diffPressure/AIR_DENSITY);
    }
    else {
      airSpeed = 0;
    }

    //voltage in V
    sharedData::meas::battVoltage.write(battVoltage, apps::timeUS());

    //value of differential air pressure from the pitot tube in Pa
    sharedData::meas::diffPressure.write(diffPressure, apps::timeUS());

    //airspeed in m/s
    sharedData::meas::airSpeed.write(airSpeed, apps::timeUS());

    chThdSleepMicroseconds(100);
  }
}
