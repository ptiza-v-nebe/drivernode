/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_HALMANAGER_H_
#define HAL_HALMANAGER_H_

// /////////////////////////////////////////////////////////////
// Common Includes
// /////////////////////////////////////////////////////////////
#include "config.h"

#include "Encoder.h"
#include "PWM.h"
#include "OutputPin.h"
#include "DCMotor.h"
#include "hal/EncoderManager.h"
#include "hal/DCMotorManager.h"
#include "stm32l4xx.h"
#include "hal/CurrentSensor.h"
#include "hal/CurrentSensorManager.h"

class HALManager {
public:
	static HALManager& getInstance();

private:
	OutputPin statusLED; ///< the status LED
	OutputPin errorLED; ///< the error LED

	I2C_HandleTypeDef i2c; ///< the I²C to be used for the SRF08 sensors


private:
	HALManager();
public:
	OutputPin& getStatusLED();
	OutputPin& getErrorLED();

	Motor& getLeftMotor();
	Motor& getRightMotor();

	void disableAllActors();

	Encoder& getLeftEncoder();
	Encoder& getRightEncoder();

	CurrentSensor& getLeftCurrentSensor();
	CurrentSensor& getRightCurrentSensor();

private:
	void initializeHal();
	void initializeEncoders();
};

#endif /* HAL_HALMANAGER_H_ */
/** @} */
