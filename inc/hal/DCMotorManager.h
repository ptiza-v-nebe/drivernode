/*
 * DCMotorManager.h
 *
 *  Created on: 2 Apr 2019
 *      Author: ivan-zarevich
 */

#ifndef HAL_DCMOTORMANAGER_H_
#define HAL_DCMOTORMANAGER_H_

#include "hal/DCMotor.h"

// ///////////////////////////////////////////////////////////////////////////////
// Driving Motors
// ///////////////////////////////////////////////////////////////////////////////

#define IN2B_Pin GPIO_PIN_15
#define IN2B_GPIO_Port GPIOB
#define IN2A_Pin GPIO_PIN_13
#define IN2A_GPIO_Port GPIOB

/**
 * the timer used for left motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define LEFT_TIMER TIM1

/**
 * the timer channel used for left motor
 */
static constexpr auto LEFT_CHANNEL = TIM_CHANNEL_1;

/**
 * the timer used for right motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define RIGHT_TIMER TIM1

/**
 * timer channel used for right motor
 */
static constexpr auto RIGHT_CHANNEL = TIM_CHANNEL_3;

/**
 * should the left motor be inverted
 */
static constexpr bool LEFT_MOTOR_INVERT = true;

/**
 * should the right motor be inverted
 */
static constexpr bool RIGHT_MOTOR_INVERT = false;

class DCMotorManager {
private:
	void initializeMotors();
	PWM leftMotorPWM;
	PWM rightMotorPWM;
	DCMotor leftMotor;
	DCMotor rightMotor;
	TIM_HandleTypeDef htim1;
public:
	DCMotorManager();
	static DCMotorManager& getInstance();
	Motor& getLeftMotor();
	Motor& getRightMotor();
	void initializeMotorsPWM();
	void Error_Handler();
	void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
	void MX_TIM1_Init(TIM_HandleTypeDef& htim1);
	void disableAllActors();
};

#endif /* HAL_DCMOTORMANAGER_H_ */
