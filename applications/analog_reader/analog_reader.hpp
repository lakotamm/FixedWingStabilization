#pragma once
#include <ch.hpp>

//defines for the voltage divider
#define VOLTAGE_COEF 173.6 //this value is defined by the voltage divider

//defines for the Diff Speed senzor
#define ADC_SUPPLY_VOLT 3.287
#define ADC_COEF ADC_SUPPLY_VOLT/4096

#define BAROMETER_CENTER 3238
#define BAROMETER_GAIN 2.25 // default 2.5
#define AIR_DENSITY 1.225 // kg/m^3

class AnalogReader : public chibios_rt::BaseStaticThread<2048> {
private:
  float battVoltageRaw;
  float diffPressureRaw;

public:
  AnalogReader() : battVoltageRaw(0), diffPressureRaw(BAROMETER_CENTER) {}
  void main();

};
