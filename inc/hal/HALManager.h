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
#include "stm32l4xx.h"

class HALManager {
public:
	static HALManager& getInstance();

private:
	PWM leftMotorPWM;
	PWM rightMotorPWM;

	DCMotor leftMotor;
	DCMotor rightMotor;

	OutputPin statusLED; ///< the status LED
	OutputPin errorLED; ///< the error LED

	I2C_HandleTypeDef i2c; ///< the I²C to be used for the SRF08 sensors

	EncoderManager encoderManager;
	TIM_HandleTypeDef htim1;

private:
	HALManager();
public:
	EncoderManager& getEncoderManager();
	OutputPin& getStatusLED();
	OutputPin& getErrorLED();

	Motor& getLeftMotor();
	Motor& getRightMotor();
	PWM& getLeftMotorPWM();
	PWM& getRightMotorPWM();

	void disableAllActors();
	Encoder& getLeftEncoder();
	Encoder& getRightEncoder();

private:
	void initializeHal();
	void initializeEncoders();

	//motors
	void initializeMotorsPWM();
	void Error_Handler();
	void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
	void MX_TIM1_Init(TIM_HandleTypeDef& htim1);

};

#endif /* HAL_HALMANAGER_H_ */
/** @} */
