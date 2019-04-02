/*
 * EncoderManager.cpp
 *
 *  Created on: 1 Apr 2019
 *      Author: ivan-zarevich
 */

#include "hal/EncoderManager.h"
#include "cstdio"

// ///////////////////////////////////////////////////////////////////////////////
// ISRs
// ///////////////////////////////////////////////////////////////////////////////

extern "C" {

void EXTI9_5_IRQHandler() {
	//printf("EXTI9_5_IRQHandler\n\r");
    // handles left encoder A+B
	HAL_GPIO_EXTI_IRQHandler(RIGHT_ENCODER_A | RIGHT_ENCODER_B);
}
void EXTI15_10_IRQHandler() {
	//printf("EXTI15_10_IRQHandler\n\r");
    // handles right encoder A+B
	HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_A | LEFT_ENCODER_B);

}
}

void HAL_GPIO_EXTI_Callback(uint16_t pins) {
    static Encoder& left = EncoderManager::getInstance().getLeftEncoder();
    if ((pins & LEFT_ENCODER_A) || (pins & LEFT_ENCODER_B)) {
        left.update();
    }

    static Encoder& right = EncoderManager::getInstance().getRightEncoder();
    if ((pins & RIGHT_ENCODER_A) || (pins & RIGHT_ENCODER_B)) {
        right.update();
    }
}

EncoderManager::EncoderManager() :
		leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B, LEFT_ENCODER_INVERT), //
		rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B, RIGHT_ENCODER_INVERT){
	initializeEncoders();
}

/**
* Initializes the encoder GPIOs and IRQs
*/
void EncoderManager::initializeEncoders() {
    //setup gpios for alternative function as interrupts
    GPIO_InitTypeDef gpio_left = getDefaultGPIO();
    gpio_left.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_left.Pin = LEFT_ENCODER_A | LEFT_ENCODER_B;

    GPIO_InitTypeDef gpio_right = getDefaultGPIO();
    gpio_right.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_right.Pin = RIGHT_ENCODER_A | RIGHT_ENCODER_B;

    HAL_GPIO_Init(LEFT_ENCODER_GPIO, &gpio_left);
    HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right);

    //enable interrupts

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, ENCODERS_PREEMPTION_PRIORITY,
                    ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY,
                    ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/**
 * @return the Singleton instance of this class.
 */
EncoderManager& EncoderManager::getInstance() {
    static EncoderManager instance;
    return instance;
}

/**
 * @return reference to the left encoder
 */
Encoder& EncoderManager::getLeftEncoder() {
    return leftEncoder;
}

/**
 * @return reference to the right encoder
 */
Encoder& EncoderManager::getRightEncoder() {
    return rightEncoder;
}
