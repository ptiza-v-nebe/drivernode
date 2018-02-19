/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/HALManagerBigRobot.h>
#include "hal/util.h"
#include "hal/interupts.h"

#define LEFT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_10;
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_11;

#define RIGHT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_10;
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_11;

extern "C" {
void EXTI15_10_IRQHandler() {
    // handles left encoder
    HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_A);
    HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_B);
}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if (pin == LEFT_ENCODER_A || pin == LEFT_ENCODER_B) {
        // update left encoder
        HALManagerBigRobot::getInstance().getLeftEncoder().update();
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

HALManagerBigRobot& HALManagerBigRobot::getInstance() {
    static HALManagerBigRobot instance;
    return instance;
}

HALManagerBigRobot::HALManagerBigRobot() :
        leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B), rightEncoder(
        RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B) {
    initializeHal();
}

Encoder& HALManagerBigRobot::getLeftEncoder() {
    return leftEncoder;
}

Encoder& HALManagerBigRobot::getRightEncoder() {
    return rightEncoder;
}

void HALManagerBigRobot::initializeHal() {
    initializeEncoders();
}

void HALManagerBigRobot::initializeEncoders() {
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    GPIO_InitTypeDef gpio_left = getDefaultGPIO();
    gpio_left.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_left.Pin = LEFT_ENCODER_A | LEFT_ENCODER_B;

    GPIO_InitTypeDef gpio_right = getDefaultGPIO();
    gpio_right.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_right.Pin = RIGHT_ENCODER_A | RIGHT_ENCODER_B;

    HAL_GPIO_Init(LEFT_ENCODER_GPIO, &gpio_left);
    //HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY, ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
/** @} */
