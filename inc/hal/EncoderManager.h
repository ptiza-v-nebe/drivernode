/*
 * EncoderManager.h
 *
 *  Created on: 1 Apr 2019
 *      Author: ivan-zarevich
 */

#ifndef HAL_ENCODERMANAGER_H_
#define HAL_ENCODERMANAGER_H_

#include "error.h"
#include "hal/util.h"
#include "hal/interupts.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"
#include <cstdint>
#include "hal/Encoder.h"

// ///////////////////////////////////////////////////////////////////////////////
// Encoders
// ///////////////////////////////////////////////////////////////////////////////
/**
 * GPIO Bank for left encoder.
 * @attention when changing this, also change RCC clock enable in
 *            HALManager::initializeEncoder!
 */
#define LEFT_ENCODER_GPIO GPIOA

/**
 * channel a pin for left encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_11;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_10;

/**
 * should the left encoder be inverted?
 */
static constexpr bool LEFT_ENCODER_INVERT = false;

/**
 * GPIO Bank for right encoder.
 * @attention when changing this, also change RCC clock enable in
 *            HALManager::initializeEncoder!
 */
#define RIGHT_ENCODER_GPIO GPIOA

/**
 * channel a pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_9;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_8;

/**
 * should the right encoder be inverted.
 */
static constexpr bool RIGHT_ENCODER_INVERT = false;

class EncoderManager {
private:
    Encoder leftEncoder; ///< the left encoder used for odometry
    Encoder rightEncoder; ///< the right encoder used for odometry
public:
    static EncoderManager& getInstance();
	EncoderManager();
	void initializeEncoders();
	Encoder& getLeftEncoder();
	Encoder& getRightEncoder();
};

#endif /* HAL_ENCODERMANAGER_H_ */
