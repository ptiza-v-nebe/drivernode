/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */


#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * Select Robot (only one!!!)
 */
#define BIG_ROBOT
//#define SMALL_ROBOT

/**
 * Should the LED blink to signal that the Scheduler is running
 */
#define BLINK_LED

/**
 * Should the UART-Messages be converted to be human readable
 */
#define HUMAN_MODE


/**
 * Enable to generate more output in the MessageParser.
 * only effective if HUMAN_MODE is enabled
 */
#define DEBUG_PARSER

/**
 * Enable to generate more output in the DynamixelCOM class
 */
#define DEBUG_DYNAMIXEL_COM


#endif /* CONFIG_H_ */
