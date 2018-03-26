/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/HALManager.h>
#ifdef BIG_ROBOT
#include "hal/util.h"
#include "hal/interupts.h"

#define LEFT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_10; // changing this might require another IRQ!
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_11; // changing this might require another IRQ!

//TODO: right encoder
#define RIGHT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_10; // changing this might require another IRQ!
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_11; // changing this might require another IRQ!

#define LEFT_MOTOR_DIRECTION_GPIO GPIOA
static constexpr uint16_t LEFT_MOTOR_DIRECTION_PIN = GPIO_PIN_10;
static constexpr uint8_t LEFT_MOTOR_DAC_CHANNEL = DAC_CHANNEL_1;
static constexpr uint16_t LEFT_MOTOR_DAC_PIN = GPIO_PIN_4;

// TODO: right motor
#define RIGHT_MOTOR_DIRECTION_GPIO GPIOA
static constexpr uint16_t RIGHT_MOTOR_DIRECTION_PIN = GPIO_PIN_10;
static constexpr uint8_t RIGHT_MOTOR_DAC_CHANNEL = DAC_CHANNEL_1;
static constexpr uint16_t RIGHT_MOTOR_DAC_PIN = GPIO_PIN_4;

extern "C" {
void EXTI15_10_IRQHandler() {
	// handles left encoder
	HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_A);
	HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_B);

	//TODO: right encoder
}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == LEFT_ENCODER_A || pin == LEFT_ENCODER_B) {
		// update left encoder
		HALManager::getInstance().getLeftEncoder().update();
	}

	//TODO: right encoder
}

HALManager& HALManager::getInstance() {
	static HALManager instance;
	return instance;
}

HALManager::HALManager() :
		leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B), //
		rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B), //
		leftMotor(LEFT_MOTOR_DAC_CHANNEL, LEFT_MOTOR_DAC_PIN, //
				LEFT_MOTOR_DIRECTION_GPIO, LEFT_MOTOR_DIRECTION_PIN), //
		rightMotor(RIGHT_MOTOR_DAC_CHANNEL, RIGHT_MOTOR_DAC_PIN, //
				RIGHT_MOTOR_DIRECTION_GPIO, RIGHT_MOTOR_DIRECTION_PIN), //
		srf08(
				{ { &i2c, 0xEE }, { &i2c, 0xE2 }, { &i2c, 0xF2 }, { &i2c, 0xF2 } }) { //
	initializeHal();
}

Encoder& HALManager::getLeftEncoder() {
	return leftEncoder;
}

Encoder& HALManager::getRightEncoder() {
	return rightEncoder;
}

void HALManager::initializeHal() {
	initializeEncoders();
	initializeI2C();
}

Motor& HALManager::getLeftMotor() {
	return leftMotor;
}

Motor& HALManager::getRightMotor() {
	return rightMotor;
}

SRF08* HALManager::getSRF08s() {
	return srf08;
}

void HALManager::initializeEncoders() {
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	GPIO_InitTypeDef gpio_left = getDefaultGPIO();
	gpio_left.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpio_left.Pin = LEFT_ENCODER_A | LEFT_ENCODER_B;

	GPIO_InitTypeDef gpio_right = getDefaultGPIO();
	gpio_right.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpio_right.Pin = RIGHT_ENCODER_A | RIGHT_ENCODER_B;

	HAL_GPIO_Init(LEFT_ENCODER_GPIO, &gpio_left);
	//HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right); //TODO: right encoder

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY,
			ENCODERS_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HALManager::initializeI2C() {
	i2c.Instance = I2C2;
	i2c.Init.Timing = 0x10909CEC;
	i2c.Init.OwnAddress1 = 0;
	i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.OwnAddress2 = 0;
	i2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	__HAL_RCC_I2C2_CLK_ENABLE()
	;
	if (HAL_I2C_Init(&i2c) != HAL_OK) {
		//TODO: error handling
		while (1)
			;
	}

	GPIO_InitTypeDef i2cGPIO = getDefaultGPIO();
	i2cGPIO.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	i2cGPIO.Mode = GPIO_MODE_AF_OD;
	i2cGPIO.Alternate = GPIO_AF4_I2C2;

	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	HAL_GPIO_Init(GPIOB, &i2cGPIO);
}

#endif
/** @} */
