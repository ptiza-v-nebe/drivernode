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
#include "error.h"

// ///////////////////////////////////////////////////////////////////////////////
// Hardware configuration
// ///////////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////////
// Encoders
// ///////////////////////////////////////////////////////////////////////////////
/**
 * GPIO Bank for left encoder.
 * @attention when changing this, also change RCC clock enable in
 *            HALManager::initializeEncoder!
 */
#define LEFT_ENCODER_GPIO GPIOC

/**
 * channel a pin for left encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_12;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_13;

/**
 * should the left encoder be inverted?
 */
static constexpr bool LEFT_ENCODER_INVERT = false;

/**
 * GPIO Bank for right encoder.
 * @attention when changing this, also change RCC clock enable in
 *            HALManager::initializeEncoder!
 */
#define RIGHT_ENCODER_GPIO GPIOC

/**
 * channel a pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_2;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_3;

/**
 * should the right encoder be inverted.
 */
static constexpr bool RIGHT_ENCODER_INVERT = true;


// ///////////////////////////////////////////////////////////////////////////////
// Driving Motors
// ///////////////////////////////////////////////////////////////////////////////

/**
 * ID of the left motor
 */
static constexpr uint8_t LEFT_MOTOR_ID = 1;

/**
 * ID of the right motor
 */
static constexpr uint8_t RIGHT_MOTOR_ID = 2;

/**
 * should the left motor be inverted
 */
static constexpr bool LEFT_MOTOR_INVERT = false;

/**
 * should the right motor be inverted
 */
static constexpr bool RIGHT_MOTOR_INVERT = true;


/**
 * which USART to use for the motors.
 * @attention when changing this, also change RCC clock enable as well as GPIO bank and
 *            pin in HALManager::initializeMotorUART
 */
#define MOTOR_USART USART3

/**
 * which GPIO bank to use for the motor UART.
 * @attention this depends on MOTOR_UART! When changing this, also change RCC clock
 *            enable in HALManager::initializeMotorUART
 */
#define MOTOR_GPIO GPIOC

/**
 * which baudrate to use for communicating with the motors
 */
static constexpr uint32_t MOTOR_UART_BAUDRATE = 38400;

/**
 * UART TX pin.
 * @attention this depends on MOTOR_UART!
 */
static constexpr uint16_t MOTOR_UART_TX = GPIO_PIN_10;

/**
 * UART RX pin.
 * @attention this depends on MOTOR_UART!
 */
static constexpr uint16_t MOTOR_UART_RX = GPIO_PIN_11;


// ///////////////////////////////////////////////////////////////////////////////
// Ultrasonic sensors
// ///////////////////////////////////////////////////////////////////////////////

/**
 * ID of the first SRF08
 */
static constexpr uint8_t SRF08_1_ID = 0xEE;

/**
 * ID of the second SRF08
 */
static constexpr uint8_t SRF08_2_ID = 0xEE;

/**
 * ID of the third SRF08
 */
static constexpr uint8_t SRF08_3_ID = 0xEE;

/**
 * ID of the fourth SRF08
 */
static constexpr uint8_t SRF08_4_ID = 0xEE;

/**
 * which I²C controller to use.
 * @attention when changing this also change RCC clock enable and GPIO bank and pins in
 *            HALManager::initializeI2C
 */
#define SRF08_I2C I2C2

/**
 * which GPIO bank to use for SRF08.
 * @attention this depends on SRF08_I2C, when changing this also change RCC clock enable
 *            in HALManager::initializeI2C
 */
#define SRF08_GPIO GPIOB

/**
 * which pin to use for SCL signal.
 * @attention this depends on SRF08_I2C
 */
static constexpr uint16_t SRF08_SCL = GPIO_PIN_13;

/**
 * wich pn to use for SDA signal.
 * @attention depends on SRF08_I2C
 */
static constexpr uint16_t SRF08_SDA = GPIO_PIN_14;


// ///////////////////////////////////////////////////////////////////////////////
// ISRs
// ///////////////////////////////////////////////////////////////////////////////

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


// ///////////////////////////////////////////////////////////////////////////////
// Code
// ///////////////////////////////////////////////////////////////////////////////

/**
 * @return the Singleton instance of this class.
 */
HALManager& HALManager::getInstance() {
    static HALManager instance;
    return instance;
}

/**
 * Constructs and initializes the HALManager.
 */
HALManager::HALManager() :
        leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B,
                LEFT_ENCODER_INVERT), //
        rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B,
                RIGHT_ENCODER_INVERT), //
        leftMotor(&motorUART, LEFT_MOTOR_ID, LEFT_MOTOR_INVERT), //
        rightMotor(&motorUART, RIGHT_MOTOR_ID, RIGHT_MOTOR_INVERT), //
        srf08(
                { { &i2c, SRF08_1_ID }, { &i2c, SRF08_2_ID }, { &i2c, SRF08_3_ID }, { &i2c, SRF08_4_ID } }), //
        i2c { }, motorUART { } { //
    initializeHal();

    leftMotor.disableAndStop();
    rightMotor.disableAndStop();
}

/**
 * @return reference to the left encoder
 */
Encoder& HALManager::getLeftEncoder() {
    return leftEncoder;
}

/**
 * @return reference to the right encoder
 */
Encoder& HALManager::getRightEncoder() {
    return rightEncoder;
}

/**
 * initializes the necessary low level components
 */
void HALManager::initializeHal() {
    initializeMotorUART();
    initializeEncoders();
    initializeI2C();
}

/**
 * @return reference to the left driving motor
 */
Motor& HALManager::getLeftMotor() {
    return leftMotor;
}

/**
 * @return reference to the right driving motor
 */
Motor& HALManager::getRightMotor() {
    return rightMotor;
}

/**
 * @return pointer to the first SRF08. Refer to SRF08_COUNT for array size.
 */
SRF08* HALManager::getSRF08s() {
    return srf08;
}

/**
 * Initializes the encoder GPIOs and IRQs
 */
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

/**
 * Initializes the USART and GPIO for the motors
 */
void HALManager::initializeMotorUART() {
    motorUART.Instance = MOTOR_USART;
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

    HAL_GPIO_Init(MOTOR_GPIO, &uart_gpio);
    TRY(HAL_UART_Init(&motorUART));
}

/**
 * Initializes the I²C and GPIO for the SRF08s
 */
void HALManager::initializeI2C() {
    i2c.Instance = SRF08_I2C;
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
    TRY(HAL_I2C_Init(&i2c));

    GPIO_InitTypeDef i2cGPIO = getDefaultGPIO();
    i2cGPIO.Pin = SRF08_SCL | SRF08_SDA;
    i2cGPIO.Mode = GPIO_MODE_AF_OD;
    i2cGPIO.Alternate = GPIO_AF4_I2C2;

    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    HAL_GPIO_Init(SRF08_GPIO, &i2cGPIO);
}

#endif
/** @} */
