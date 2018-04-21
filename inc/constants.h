/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */


#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
 * The default timeout for blocking HAL Operations
 */
constexpr int DEFAULT_TIMEOUT_MS = 1;

/**
 * Controlsystem constants
 */
constexpr double CONTROLLER_SAMPLING_TIME = 0.005; // in seconds
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN = 1.070208;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME = 0.0514623;
constexpr float CONTROLLER_SYSTEM_LEFT_WHEEL_KP = 2.7/CONTROLLER_SYSTEM_LEFT_WHEEL_GAIN;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN = 1.00992;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME = 0.0419616;
constexpr float CONTROLLER_SYSTEM_RIGHT_WHEEL_KP = 2.75/CONTROLLER_SYSTEM_RIGHT_WHEEL_GAIN;
constexpr float CONTROLLER_SYSTEM_DELAYTIME = 2.0*(CONTROLLER_SYSTEM_LEFT_WHEEL_KP+CONTROLLER_SYSTEM_RIGHT_WHEEL_KP)/2;
constexpr float CONTROLLER_POSITION_KP = 0.75;
constexpr float CONTROLLER_ANGLE_KP = 1.0;

#endif /* CONSTANTS_H_ */
