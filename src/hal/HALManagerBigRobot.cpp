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
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_4; // changing this might require another IRQ!
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_5; // changing this might require another IRQ!

//TODO: right encoder
#define RIGHT_ENCODER_GPIO GPIOC // when changing, also change clock enable in initializeEncoders!
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_4; // changing this might require another IRQ!
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_5; // changing this might require another IRQ!

static constexpr uint8_t LEFT_MOTOR_ID = 1;
static constexpr uint8_t RIGHT_MOTOR_ID = 2;
static constexpr bool LEFT_MOTOR_INVERT = false;
static constexpr bool RIGHT_MOTOR_INVERT = true;

// Change USART and GPIO Port in initializeMotorUART();
static constexpr uint32_t MOTOR_UART_BAUDRATE = 38400;
static constexpr uint16_t MOTOR_UART_TX = GPIO_PIN_10;
static constexpr uint16_t MOTOR_UART_RX = GPIO_PIN_11;

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
        leftMotor(&motorUART, LEFT_MOTOR_ID, LEFT_MOTOR_INVERT), //
        rightMotor(&motorUART, RIGHT_MOTOR_ID, RIGHT_MOTOR_INVERT) { //
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
    initializeMotorUART();
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
    //HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right); //TODO: right encoder

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HALManager::initializeMotorUART() {
    motorUART.Instance = USART3;
    motorUART.Init.BaudRate = MOTOR_UART_BAUDRATE;
    motorUART.Init.Parity = UART_PARITY_NONE;
    motorUART.Init.Mode = UART_MODE_TX_RX;
    motorUART.Init.StopBits = UART_STOPBITS_1;
    motorUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    motorUART.Init.WordLength = UART_WORDLENGTH_8B;
    motorUART.Init.OneBitSampling = UART_ONEBIT_SAMPLING_ENABLED;
    motorUART.Init.OverSampling = UART_OVERSAMPLING_16;

    motorUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    GPIO_InitTypeDef uart_gpio = getDefaultGPIO();
    uart_gpio.Pin = MOTOR_UART_TX | MOTOR_UART_RX;
    uart_gpio.Mode = GPIO_MODE_AF_OD;
    uart_gpio.Alternate = GPIO_AF7_USART3;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    HAL_GPIO_Init(GPIOC, &uart_gpio);
    HAL_UART_Init (&motorUART);
}

#endif
/** @} */
