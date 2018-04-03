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
        leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B,
                LEFT_ENCODER_SIGN), //
        rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B,
                RIGHT_ENCODER_SIGN), //
        leftMotor(&motorUART, LEFT_MOTOR_ID, LEFT_MOTOR_INVERT), //
        rightMotor(&motorUART, RIGHT_MOTOR_ID, RIGHT_MOTOR_INVERT), //
        srf08(
                { { &i2c, 0xEE }, { &i2c, 0xE2 }, { &i2c, 0xF2 }, { &i2c, 0xF2 } }), //
        i2c { }, motorUART { } { //
    initializeHal();

    leftMotor.disableAndStop();
    rightMotor.disableAndStop();
}

Encoder& HALManager::getLeftEncoder() {
    return leftEncoder;
}

Encoder& HALManager::getRightEncoder() {
    return rightEncoder;
}

void HALManager::initializeHal() {
    initializeMotorUART();
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

    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    __HAL_RCC_USART3_CLK_ENABLE()
    ;

    HAL_GPIO_Init(GPIOC, &uart_gpio);
    HAL_UART_Init(&motorUART);
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
