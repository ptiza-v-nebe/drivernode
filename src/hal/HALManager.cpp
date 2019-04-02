/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/HALManager.h>
#include "hal/util.h"
#include "error.h"

// ///////////////////////////////////////////////////////////////////////////////
// Hardware configuration
// ///////////////////////////////////////////////////////////////////////////////



// ///////////////////////////////////////////////////////////////////////////////
// Constructor
// ///////////////////////////////////////////////////////////////////////////////
/**
 * Constructs and initializes the HALManager.
 */
HALManager::HALManager() :
		statusLED(GPIOB, GPIO_PIN_6, GPIO_PIN_SET), //
		errorLED(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
{
	initializeHal();
	statusLED.enable();
	errorLED.enable();
	statusLED.setOn();
	disableAllActors();
}

// ///////////////////////////////////////////////////////////////////////////////
// Common Code
// ///////////////////////////////////////////////////////////////////////////////
/**
 * @return the Singleton instance of this class.
 */
HALManager& HALManager::getInstance() {
	static HALManager instance;
	return instance;
}

/**
 * initializes the necessary low level components
 */
void HALManager::initializeHal() {

}

/**
 * @return reference to the left driving motor
 */
 Motor& HALManager::getLeftMotor() {
	 return DCMotorManager::getInstance().getLeftMotor();
 }

/**
 * @return reference to the right driving motor
 */

 Motor& HALManager::getRightMotor() {
	 return DCMotorManager::getInstance().getRightMotor();
 }


/**
 * @return reference to the status LED
 */
OutputPin& HALManager::getStatusLED() {
	return statusLED;
}

/**
 * @return reference to the error LED
 */
OutputPin& HALManager::getErrorLED() {
	return errorLED;
}

/**
 * @return reference to the left encoder
 */
Encoder& HALManager::getLeftEncoder() {
	return EncoderManager::getInstance().getLeftEncoder();
}

/**
 * @return reference to the right encoder
 */
Encoder& HALManager::getRightEncoder() {
	return EncoderManager::getInstance().getRightEncoder();
}

/**
 * disables all actors
 */
void HALManager::disableAllActors() {
	DCMotorManager::getInstance().disableAllActors();
}

CurrentSensor& HALManager::getLeftCurrentSensor() {
	return CurrentSensorManager::getInstance().getLeftCurrentSensor();
}

CurrentSensor& HALManager::getRightCurrentSensor() {
	return CurrentSensorManager::getInstance().getRightCurrentSensor();
}
