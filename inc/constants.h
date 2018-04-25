/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */


#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "config.h"
#include "position/Angle.h"

/**
 * The default timeout for blocking HAL Operations
 */
constexpr int DEFAULT_TIMEOUT_MS = 1;

/**
 * Controlsystem constants
 */
constexpr float CONTROLLER_SAMPLING_TIME = 0.01; // in seconds

#ifdef BIG_ROBOT
constexpr float MOTORCONSTANT = 14*60*(1/(2*PI*0.03));

constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN = 1.0154575;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME = 0.111984;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_KP = 0.66*(0.1/CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN)/CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME;

constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN = 1.1231625;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME = 0.161572;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_KP = 0.66*(0.175/CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN)/CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME;

constexpr float CONTROLLER_SYSTEM_DELAYTIME = 2.0*(CONTROLLER_SYSTEM_LEFT_WHEEL_KP+CONTROLLER_SYSTEM_RIGHT_WHEEL_KP)/2;
constexpr float CONTROLLER_POSITION_KP = 0.75;
constexpr float CONTROLLER_ANGLE_KP = 1.0;
#else
constexpr float MOTORCONSTANT = 32*200*(1/(2*PI*0.03));

constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN = 1.0547525;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME = 0.127908;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_KP = (0.1/CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN)/CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME;

constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN = 1.000985;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME = 0.230022;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_KP = (0.295/CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN)/CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME;

constexpr float CONTROLLER_SYSTEM_DELAYTIME = 2.0*(CONTROLLER_SYSTEM_LEFT_WHEEL_KP+CONTROLLER_SYSTEM_RIGHT_WHEEL_KP)/2;
constexpr float CONTROLLER_POSITION_KP = 0.75;
constexpr float CONTROLLER_ANGLE_KP = 1.0;
#endif

/**
 * ultrasonic backwardvision constants
 */
constexpr uint16_t ULTRASONIC_LOWERTHRESHOLD = 20;
constexpr uint16_t ULTRASONIC_HIGHERTHRESHOLD = 30;

#endif /* CONSTANTS_H_ */
