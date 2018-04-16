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
constexpr double CONTROLLER_SYSTEM_POSITION_GAIN = 0.22;
constexpr double CONTROLLER_SYSTEM_POSITION_DELAYTIME = 0.05;
constexpr double CONTROLLER_SYSTEM_ANGLE_GAIN = 0.0847;
constexpr double CONTROLLER_SYSTEM_ANGLE_DELAYTIME = 0.055;

#endif /* CONSTANTS_H_ */
