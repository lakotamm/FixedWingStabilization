/*
 * autopilotconf.h
 *
 *  Created on: May 7, 2018
 *      Author: das
 */

#ifndef AUTOPILOTCONF_H_
#define AUTOPILOTCONF_H_

//definitions of physically maximally possible angular rates in degrees/s
//test the airplane and insert values here
#define ROLL_MAX_ANGULAR_RATE   360
#define PITCH_MAX_ANGULAR_RATE  180
#define YAW_MAX_ANGULAR_RATE    0

//converts maximum values into radians
#define ROLL_ANGULAR_GAIN       ROLL_MAX_ANGULAR_RATE/57.3
#define PITCH_ANGULAR_GAIN      PITCH_MAX_ANGULAR_RATE/57.3
#define YAW_ANGULAR_GAIN        YAW_MAX_ANGULAR_RATE/57.3

//air speeds in m/s
#define MAX_AIRSPEED            20
#define MIN_AIRSPEED            0

#define TUNE_AIRSPEED_PID       true

#define TUNE_PITCH_RATE_PIFF    false
#define TUNE_ROLL_RATE_PIFF     false
#define TUNE_YAW_RATE_PIFF      false

#define TUNE_PITCH_ORIENT_PID   false
#define TUNE_ROLL_ORIENT_PID    false
#define TUNE_YAW_ORIENT_PID     false


#endif /* AUTOPILOTCONF_H_ */
