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
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_12; // changing this might require another IRQ!
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_13; // changing this might require another IRQ!
static constexpr int LEFT_ENCODER_SIGN = 1;

#define RIGHT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_2; // changing this might require another IRQ!
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_3; // changing this might require another IRQ!
static constexpr int RIGHT_ENCODER_SIGN = -1;

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
    // handles left encoder A+B
    HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_A | LEFT_ENCODER_B);
}
void EXTI2_IRQHandler() {
    // handle right encoder A
    HAL_GPIO_EXTI_IRQHandler(RIGHT_ENCODER_A);
}
void EXTI3_IRQHandler() {
    // handles right encoder B
    HAL_GPIO_EXTI_IRQHandler(RIGHT_ENCODER_B);
}
}

void HAL_GPIO_EXTI_Callback(uint16_t pins) {
    if ((pins & LEFT_ENCODER_A) || (pins & LEFT_ENCODER_B)) {
        // update left encoder
        HALManager::getInstance().getLeftEncoder().update();
    }
    if ((pins & RIGHT_ENCODER_A) || (pins & RIGHT_ENCODER_B)) {
        // update right encoder
        HALManager::getInstance().getRightEncoder().update();
    }
}

HALManager& HALManager::getInstance() {
    static HALManager instance;
    return instance;
}

HALManager::HALManager() :
        leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B, LEFT_ENCODER_SIGN), //
        rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B, RIGHT_ENCODER_SIGN), //
        leftMotor(LEFT_MOTOR_DAC_CHANNEL, LEFT_MOTOR_DAC_PIN, //
                LEFT_MOTOR_DIRECTION_GPIO, LEFT_MOTOR_DIRECTION_PIN), //
        rightMotor(RIGHT_MOTOR_DAC_CHANNEL, RIGHT_MOTOR_DAC_PIN, //
                RIGHT_MOTOR_DIRECTION_GPIO, RIGHT_MOTOR_DIRECTION_PIN) { //
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
}

Motor& HALManager::getLeftMotor() {
    return leftMotor;
}

Motor& HALManager::getRightMotor() {
    return rightMotor;
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
    HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    HAL_NVIC_SetPriority(EXTI2_IRQn, ENCODERS_PREEMPTION_PRIORITY,
                ENCODERS_SUB_PRIORITY);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);
        HAL_NVIC_SetPriority(EXTI3_IRQn, ENCODERS_PREEMPTION_PRIORITY,
                    ENCODERS_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}
#endif
/** @} */
