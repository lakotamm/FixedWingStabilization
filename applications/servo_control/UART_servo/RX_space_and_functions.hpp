#include "Porting_layer.hpp"
#include "MASTER_UART_RX_msg.h"

    //Header
struct header_t {
  friend class UARTservo;
  private:
    bool motorState;
    bool warnings;
    bool errors;
    bool permanentErrors;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent(
       bool setMotorState,
       bool setWarnings,
       bool setErrors,
       bool setPermanentErrors){
      motorState = setMotorState;
      warnings = setWarnings;
      errors = setErrors;
      permanentErrors = setPermanentErrors;

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:
//TODO finish comment
    bool getMotorState(){
      msgRead=TRUE;
      return motorState;
    }

//TODO finish comment
    bool isWarnings(){
      msgRead=TRUE;
      return warnings;
    }

//TODO finish comment
    bool isErrors(){
      msgRead=TRUE;
      return errors;
    }

//TODO finish comment
    bool isPermanentErrors(){
      msgRead=TRUE;
      return permanentErrors;
    }


//TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }
//TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};

    //Feedback message
struct feedback_t {
  friend class UARTservo;
  private:
    int16_t position;
    int16_t posError;
    int16_t torque;
    int16_t speed;
    int16_t temperature;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent(
       int16_t newPosition,
       int16_t newPosError,
       int16_t newTorque,
       int16_t newSpeed,
       int16_t newTemperature){
      position = newPosition;
      posError = newPosError;
      torque = newTorque;
      speed = newSpeed;
      temperature = newTemperature;

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:
//TODO finish comment
    int16_t getPosition(){
      msgRead=TRUE;
      return position;
    };

//TODO finish comment
    int16_t getPosError(){
      msgRead=TRUE;
      return posError;
    };

//TODO finish comment
    int16_t getTorque(){
      msgRead=TRUE;
      return torque;
    };

//TODO finish comment
    int16_t getSpeed(){
      msgRead=TRUE;
      return speed;
    };

//TODO finish comment
    int16_t getTemperature(){
      msgRead=TRUE;
      return temperature;
    };

//TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }

//TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};

    //Data message
struct service_data_t {
  friend class UARTservo;
  private:
    uint32_t elSerialNumber;
    uint8_t userString[RX_USER_STRING_LENGTH];
    uint32_t mechSerialNumber;
    uint32_t runTime;
    uint16_t SWversion;
    uint16_t servoWear;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent(
       uint32_t newElSerialNumber,
       uint8_t newUserString[RX_USER_STRING_LENGTH],
       uint32_t newMechSerialNumber,
       uint32_t newRunTime,
       uint16_t newSWversion,
       uint16_t newServoWear){
      elSerialNumber = newElSerialNumber;

      mechSerialNumber = newMechSerialNumber;
      runTime = newRunTime;
      SWversion = newSWversion;
      servoWear = newServoWear;

      for (uint8_t i = 0; i < RX_USER_STRING_LENGTH; i++) {
        userString[i] = newUserString[i];
      }

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:

  //TODO finish comment
    uint32_t getElSerialNumber(){
      msgRead=TRUE;
      return elSerialNumber;
    }

  //TODO finish comment
    void getUserString(uint8_t readUserString[RX_USER_STRING_LENGTH]){
      for (uint8_t i = 0; i < RX_USER_STRING_LENGTH; i++) {
        readUserString[i] = userString[i];
      }
    }

  //TODO finish comment
    uint32_t getMechSerialNumber(){
      msgRead=TRUE;
      return mechSerialNumber;
    }

  //TODO finish comment
    uint32_t getRunTime(){
      msgRead=TRUE;
      return runTime;
    }
  //TODO finish comment
    uint32_t getSWversion(){
      msgRead=TRUE;
      return SWversion;
    }

  //TODO finish comment
    uint16_t getServoWear(){
      msgRead=TRUE;
      return servoWear;
    }

  //TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }

  //TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};

    //Settings message
struct settings_t {
  friend class UARTservo;
  private:
    uint16_t pSetting;
    uint16_t iSetting;
    uint16_t dSetting;

    int16_t zeroPosition;
    int16_t minPosition;
    int16_t maxPosition;
    uint16_t currentLimit;
    uint16_t voltageLimit;

    uint8_t servoID;
    uint8_t groupID;
    uint8_t servoMode;

    uint8_t feedbackInterval;
    uint8_t dataInterval;
    uint8_t settingsInterval;
    uint8_t errorsInterval;
    uint8_t debugInterval;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent(
       uint16_t newPSetting,
       uint16_t newISetting,
       uint16_t newDSetting,

       int16_t newZeroPosition,
       int16_t newMinPosition,
       int16_t newMaxPosition,
       uint16_t newCurrentLimit,
       uint16_t newVoltageLimit,

       uint8_t newServoID,
       uint8_t newGroupID,
       uint8_t newServoMode,

       uint8_t newFeedbackInterval,
       uint8_t newDataInterval,
       uint8_t newSettingsInterval,
       uint8_t newErrorsInterval,
       uint8_t newDebugInterval){
      pSetting = newPSetting;
      iSetting = newISetting;
      dSetting = newDSetting;

      zeroPosition = newZeroPosition;
      minPosition = newMinPosition;
      maxPosition = newMaxPosition;
      currentLimit = newCurrentLimit;
      voltageLimit = newVoltageLimit;

      servoID = newServoID,
      groupID = newGroupID,
      servoMode = newServoMode,

      feedbackInterval = newFeedbackInterval;
      dataInterval = newDataInterval;
      settingsInterval = newSettingsInterval;
      errorsInterval = newErrorsInterval;
      debugInterval = newDebugInterval;

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:
//TODO finish comment
    uint16_t getPSetting(){
      msgRead = TRUE;
    return pSetting;
    }

//TODO finish comment
    uint16_t getISetting(){
      msgRead = TRUE;
    return iSetting;
    }

//TODO finish comment
    uint16_t getDSetting(){
      msgRead = TRUE;
    return dSetting;
    }

//TODO finish comment
    int16_t getZeroPosition(){
      msgRead = TRUE;
    return zeroPosition;
    }
//TODO finish comment
    int16_t getMinPosition(){
      msgRead = TRUE;
    return minPosition;
    }
//TODO finish comment
    int16_t getMaxPosition(){
      msgRead = TRUE;
    return maxPosition;
    }

//TODO finish comment
    uint16_t getCurrentLimit(){
      msgRead = TRUE;
    return currentLimit;
    }

//TODO finish comment
    uint16_t getVoltageLimit(){
      msgRead = TRUE;
    return voltageLimit;
    }

//TODO finish comment
    uint8_t getServoID(){
      msgRead = TRUE;
    return servoID;
    }

//TODO finish comment
    uint8_t getGroupID(){
      msgRead = TRUE;
    return groupID;
    }

//TODO finish comment
    uint8_t getServoMode(){
      msgRead = TRUE;
    return servoMode;
    }

//TODO finish comment
    uint8_t getFeedbackInterval(){
      msgRead = TRUE;
    return feedbackInterval;
    }

//TODO finish comment
    uint8_t getDataInterval(){
      msgRead = TRUE;
    return dataInterval;
    }

//TODO finish comment
    uint8_t getSettingsInterval(){
      msgRead = TRUE;
    return settingsInterval;
    }

//TODO finish comment
    uint8_t getErrorsInterval(){
      msgRead = TRUE;
    return errorsInterval;
    }

//TODO finish comment
    uint8_t getDebugInterval(){
      msgRead = TRUE;
    return debugInterval;
    }

//TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }

//TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};

    //Errors message
struct errors_t {
  friend class UARTservo;
  private:
    uint8_t warnings;
    uint8_t errors;
    uint8_t permanentErrors;
    uint8_t serviceIntervals;
    uint16_t eepromErrors;
    uint16_t uartErrors;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent (
        uint8_t newWarnings,
        uint8_t newErrors,
        uint8_t newPermanentErrors,
        uint8_t newServiceIntervals,
        uint16_t newEepromErrors,
        uint16_t newUartErrors){
      warnings = newWarnings;
      errors = newErrors;
      permanentErrors = newPermanentErrors;
      serviceIntervals = newServiceIntervals;
      eepromErrors = newEepromErrors;
      uartErrors = newUartErrors;

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:
//TODO add functions for reading errors

//TODO finish comment
    uint16_t getEepromErrors(){
      msgRead = TRUE;
    return eepromErrors;
    }

//TODO finish comment
    uint16_t getUartErrors(){
      msgRead = TRUE;
    return uartErrors;
    }
//TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }

//TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};

    //Debug message
struct debug_t {
  friend class UARTservo;
  private:
    uint16_t rawPosition;
    uint16_t inputVoltage;
    uint16_t outputVoltage;
    int16_t motorVoltage;
    int16_t phaseCurrent;
    uint16_t outputPower;
    uint16_t currentConsumption;
    uint16_t powerConsumption;
    uint16_t consumedCurrent;
    uint16_t consumedPower;

    bool msgRead;
    uint32_t timestamp;

//TODO finish comment
    void setContent (
        uint16_t newRawPosition,
        uint16_t newInputVoltage,
        uint16_t newOutputVoltage,
        int16_t newMotorVoltage,
        int16_t newPhaseCurrent,
        uint16_t newOutputPower,
        uint16_t newCurrentConsumption,
        uint16_t newPowerConsumption,
        uint16_t newConsumedCurrent,
        uint16_t newConsumedPower) {
      rawPosition = newRawPosition;
      inputVoltage = newInputVoltage;
      outputVoltage = newOutputVoltage;
      motorVoltage = newMotorVoltage;
      phaseCurrent = newPhaseCurrent;
      outputPower = newOutputPower;
      currentConsumption = newCurrentConsumption;
      powerConsumption = newPowerConsumption;
      consumedCurrent = newConsumedCurrent;
      consumedPower = newConsumedPower;

      msgRead = FALSE;
      timestamp = GET_SYS_TIME();
    }

  public:
//TODO finish comment
    uint16_t getRawPosition(){
      msgRead = TRUE;
    return rawPosition;
    }

//TODO finish comment
    uint16_t getInputVoltage(){
      msgRead = TRUE;
    return inputVoltage;
    }

//TODO finish comment
    uint16_t getOutputVoltage(){
      msgRead = TRUE;
    return outputVoltage;
    }

//TODO finish comment
    int16_t getMotorVoltage(){
      msgRead = TRUE;
    return motorVoltage;
    }

//TODO finish comment
    int16_t getPhaseCurrent(){
      msgRead = TRUE;
    return phaseCurrent;
    }

//TODO finish comment
    uint16_t getOutputPower(){
      msgRead = TRUE;
    return outputPower;
    }

//TODO finish comment
    uint16_t getCurrentConsumption(){
      msgRead = TRUE;
    return currentConsumption;
    }

//TODO finish comment
    uint16_t getPowerConsumption(){
      msgRead = TRUE;
    return powerConsumption;
    }

//TODO finish comment
    uint16_t getConsumedCurrent(){
      msgRead = TRUE;
    return consumedCurrent;
    }

//TODO finish comment
    uint16_t getConsumedPower(){
      msgRead = TRUE;
    return consumedPower;
    }

//TODO finish comment
    bool isMsgRead(){
      return msgRead;
    }

//TODO finish comment
    uint32_t getTimestamp(){
      return timestamp;
    }
};
