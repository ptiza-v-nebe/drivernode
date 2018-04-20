/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/HALManager.h>
#include "hal/util.h"
#include "hal/interupts.h"
#include "error.h"

// ///////////////////////////////////////////////////////////////////////////////
// Hardware configuration
// ///////////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////////
// Common
// ///////////////////////////////////////////////////////////////////////////////

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
static constexpr uint16_t LEFT_ENCODER_A = GPIO_PIN_4;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t LEFT_ENCODER_B = GPIO_PIN_6;

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
static constexpr uint16_t RIGHT_ENCODER_A = GPIO_PIN_11;

/**
 * channel b pin for right encoder.
 * @attention changing this might require using another IRQ/ISR
 */
static constexpr uint16_t RIGHT_ENCODER_B = GPIO_PIN_12;

/**
 * should the right encoder be inverted.
 */
static constexpr bool RIGHT_ENCODER_INVERT = true;

// ///////////////////////////////////////////////////////////////////////////////
// Ultrasonic sensors
// ///////////////////////////////////////////////////////////////////////////////

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
// Specific Config
// ///////////////////////////////////////////////////////////////////////////////
#ifdef BIG_ROBOT
// ///////////////////////////////////////////////////////////////////////////////
// Driving Motors
// ///////////////////////////////////////////////////////////////////////////////

/**
 * ID of the left motor
 */
static constexpr uint8_t LEFT_MOTOR_ID = 2;

/**
 * ID of the right motor
 */
static constexpr uint8_t RIGHT_MOTOR_ID = 1;

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
#define MOTOR_GPIO GPIOB

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
static constexpr uint8_t SRF08_1_ID = 0xE2;

/**
 * ID of the second SRF08
 */
static constexpr uint8_t SRF08_2_ID = 0xEE;

#endif
#ifdef SMALL_ROBOT

// ///////////////////////////////////////////////////////////////////////////////
// Stepper Motors
// ///////////////////////////////////////////////////////////////////////////////
/**
 * the timer used for left motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define LEFT_TIMER TIM5

/**
 * the timer channel used for left motor
 */
static constexpr auto LEFT_CHANNEL = TIM_CHANNEL_1;

/**
 * the GPIO bank used for left motor
 * @attention depends on LEFT_TIMER and LEFT_CHANNEL!
 */
#define LEFT_TIMER_GPIO GPIOA

/**
 * the GPIO pin used for the left timer
 * @attention depends on LEFT_TIMER and LEFT_CHANNEL!
 */
static constexpr uint16_t LEFT_TIMER_PIN = GPIO_PIN_0;

/**
 * the timer used for right motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define RIGHT_TIMER TIM2

/**
 * timer channel used for right motor
 */
static constexpr auto RIGHT_CHANNEL = TIM_CHANNEL_1;

/**
 * the GPIO bank used for right motor
 * @attention depends on RIGHT_TIMER and RIGHT_CHANNEL!
 */
#define RIGHT_TIMER_GPIO GPIOA

/**
 * the GPIO pin used for the right timer
 * @attention depends on RIGHT_TIMER and RIGHT_CHANNEL!
 */
static constexpr uint16_t RIGHT_TIMER_PIN = GPIO_PIN_5;

// ///////////////////////////////////////////////////////////////////////////////
// Shooting BLDC
// ///////////////////////////////////////////////////////////////////////////////

/**
 * the timer used for shooting bldc
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeShootingBLDC
 */
#define BLDC_TIMER TIM8

/**
 * timer channel used for shooting bldc
 */
static constexpr auto BLDC_CHANNEL = TIM_CHANNEL_1;

/**
 * the GPIO bank used for shooting bldc
 * @attention depends on BLDC_TIMER and BLDC_CHANNEL!
 */
#define BLDC_TIMER_GPIO GPIOA

/**
 * the GPIO pin used for shooting bldc
 * @attention depends on BLDC_TIMER and BLDC_CHANNEL!
 */
static constexpr uint16_t BLDC_TIMER_PIN = GPIO_PIN_7;

/**
 * the id of the dynamixel with the little arm
 */
static constexpr uint8_t DYNAMIXEL_ID = 4;

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
static constexpr uint8_t SRF08_2_ID = 0xE2;

#endif

// ///////////////////////////////////////////////////////////////////////////////
// ISRs
// ///////////////////////////////////////////////////////////////////////////////

extern "C" {
#ifdef BIG_ROBOT
void EXTI1_IRQHandler() {
    // handle scara encoder B
    HAL_GPIO_EXTI_IRQHandler(SCARA_ENCODER_B);
}
void EXTI2_IRQHandler() {
    // handle scara encoder A
    HAL_GPIO_EXTI_IRQHandler(SCARA_ENCODER_A);
}
#endif

void EXTI4_IRQHandler() {
    // handles left encoder A
    HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_A);
}
void EXTI9_5_IRQHandler() {
    // handles left encoder B (pin 6)
    HAL_GPIO_EXTI_IRQHandler(LEFT_ENCODER_B);
}
void EXTI15_10_IRQHandler() {
    // handles right encoder A+B (pins 11 and 12)
    HAL_GPIO_EXTI_IRQHandler(RIGHT_ENCODER_A | RIGHT_ENCODER_B);
}
}

void HAL_GPIO_EXTI_Callback(uint16_t pins) {
    static Encoder& left = HALManager::getInstance().getLeftEncoder();
    if ((pins & LEFT_ENCODER_A) || (pins & LEFT_ENCODER_B)) {
        left.update();
    }

    static Encoder& right = HALManager::getInstance().getRightEncoder();
    if ((pins & RIGHT_ENCODER_A) || (pins & RIGHT_ENCODER_B)) {
        right.update();
    }
#ifdef BIG_ROBOT
    static Encoder& scara =
    HALManager::getInstance().getScaraHardware().getLiftEncoder();
    if ((pins & SCARA_ENCODER_A) || (pins & SCARA_ENCODER_B)) {
        scara.update();
    }
#endif
}

// ///////////////////////////////////////////////////////////////////////////////
// Constructor
// ///////////////////////////////////////////////////////////////////////////////
/**
 * Constructs and initializes the HALManager.
 */
HALManager::HALManager() :
        leftEncoder(LEFT_ENCODER_GPIO, LEFT_ENCODER_A, LEFT_ENCODER_B,
                LEFT_ENCODER_INVERT), //
        rightEncoder(RIGHT_ENCODER_GPIO, RIGHT_ENCODER_A, RIGHT_ENCODER_B,
                RIGHT_ENCODER_INVERT), //
#ifdef BIG_ROBOT
                srf08( {  {   &i2c, SRF08_1_ID}, {&i2c, SRF08_2_ID}}), //

#endif
#ifdef SMALL_ROBOT
                srf08( { { &i2c, SRF08_1_ID }, { &i2c, SRF08_2_ID } }), //

#endif
                dynamixelCom(), //
                starterSwitch(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET, GPIO_PULLUP), //
                statusLED(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET), //
                errorLED(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET), //
                i2c { }, //
#ifdef BIG_ROBOT
                scaraHardware(dynamixelCom), //
                leftMotor(&motorUART, LEFT_MOTOR_ID, LEFT_MOTOR_INVERT),//
                rightMotor(&motorUART, RIGHT_MOTOR_ID, RIGHT_MOTOR_INVERT),//
                motorUART {} //
#endif
#ifdef SMALL_ROBOT
                leftMotorPWM(LEFT_TIMER, LEFT_CHANNEL), //
                rightMotorPWM(RIGHT_TIMER, RIGHT_CHANNEL), //
                leftMotor(leftMotorPWM, { GPIOC, GPIO_PIN_4}, //
                        { GPIOA, GPIO_PIN_1, GPIO_PIN_RESET }), //
                rightMotor(rightMotorPWM, { GPIOH, GPIO_PIN_0 }, //
                        { GPIOH, GPIO_PIN_1 }), //
                shootingBLDCPWM(BLDC_TIMER, BLDC_CHANNEL), //
                shootingBLDC(shootingBLDCPWM), //
                servo( { DYNAMIXEL_ID, dynamixelCom }) //

#endif
{ //
    initializeHal();

    statusLED.enable();
    errorLED.enable();

    statusLED.setOn();

    disableAllActors();
}

// ///////////////////////////////////////////////////////////////////////////////
// Common Code
// ///////////////////////////////////////////////////////////////////////////////
/**
 * @return the Singleton instance of this class.
 */
HALManager& HALManager::getInstance() {
    static HALManager instance;
    return instance;
}

/**
 * initializes the necessary low level components
 */
void HALManager::initializeHal() {
    initializeEncoders();
    initializeI2C();

#ifdef BIG_ROBOT
    initializeMotorUART();
    scaraHardware.initialize();
#endif
#ifdef SMALL_ROBOT
    initializeMotors();
    initializeShootingBLDC();
#endif
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
 * @return reference to the starter pin
 */
InputPin& HALManager::getStarterPin() {
    return starterSwitch;
}

/**
 * @return reference to the status LED
 */
OutputPin& HALManager::getStatusLED() {
    return statusLED;
}

/**
 * @return reference to the error LED
 */
OutputPin& HALManager::getErrorLED() {
    return errorLED;
}

/**
 * Initializes the encoder GPIOs and IRQs
 */
void HALManager::initializeEncoders() {
    GPIO_InitTypeDef gpio_left = getDefaultGPIO();
    gpio_left.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_left.Pin = LEFT_ENCODER_A | LEFT_ENCODER_B;

    GPIO_InitTypeDef gpio_right = getDefaultGPIO();
    gpio_right.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_right.Pin = RIGHT_ENCODER_A | RIGHT_ENCODER_B;

    HAL_GPIO_Init(LEFT_ENCODER_GPIO, &gpio_left);
    HAL_GPIO_Init(RIGHT_ENCODER_GPIO, &gpio_right);
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

/**
 * unmasks the interrupts
 */
void HALManager::enableISRs() {
#ifdef BIG_ROBOT
    HAL_NVIC_SetPriority(EXTI1_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
#endif

    HAL_NVIC_SetPriority(EXTI4_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, ENCODERS_PREEMPTION_PRIORITY,
            ENCODERS_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * disables all actors
 */
void HALManager::disableAllActors() {
    leftMotor.disableAndStop();
    rightMotor.disableAndStop();

#ifdef BIG_ROBOT
    scaraHardware.disable();
#endif
#ifdef SMALL_ROBOT
    shootingBLDC.disableAndStop();
    servo.disableAndStop();
#endif

}

// ///////////////////////////////////////////////////////////////////////////////
// Specific Code
// ///////////////////////////////////////////////////////////////////////////////
#ifdef BIG_ROBOT

/**
 * @return reference to the scara hardware wrapper
 */
ScaraHardware& HALManager::getScaraHardware() {
    return scaraHardware;
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

    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    __HAL_RCC_USART3_CLK_ENABLE()
    ;

    HAL_GPIO_Init(MOTOR_GPIO, &uart_gpio);
    TRY(HAL_UART_Init(&motorUART));
}

#endif
#ifdef SMALL_ROBOT
/**
 * @return reference to the bldc used for shooting
 */
ShootingBLDC& HALManager::getShootingBLDC() {
    return shootingBLDC;
}

/**
 * @return reference to the dynamixel with the little arm
 */
DynamixelAX12A& HALManager::getServo() {
    return servo;
}

/**
 * initializes the timers and gpios for the pwms for the motors
 */
void HALManager::initializeMotors() {
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOH_CLK_ENABLE()
    ;

    TIM_HandleTypeDef timer = { };
    TIM_OC_InitTypeDef channel = { };
    GPIO_InitTypeDef gpio = getDefaultGPIO();

    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    timer.Init.Prescaler = 80 - 1;
    timer.Init.Period = 1000 - 1;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;

    channel.Pulse = 500;
    channel.OCMode = TIM_OCMODE_PWM1;

    gpio.Alternate = GPIO_AF1_TIM2;
    gpio.Mode = GPIO_MODE_AF_PP;

    __HAL_RCC_TIM2_CLK_ENABLE()
    ;
    __HAL_RCC_TIM5_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;

    // initialize left
    timer.Instance = LEFT_TIMER;
    gpio.Pin = LEFT_TIMER_PIN;

    HAL_TIM_PWM_Init(&timer);
    HAL_TIM_PWM_ConfigChannel(&timer, &channel, LEFT_CHANNEL);
    HAL_GPIO_Init(LEFT_TIMER_GPIO, &gpio);

    HAL_TIM_PWM_Start(&timer, LEFT_CHANNEL);
    leftMotorPWM.disable();

    // initialize right
    timer.Instance = RIGHT_TIMER;
    gpio.Pin = RIGHT_TIMER_PIN;

    HAL_TIM_PWM_Init(&timer);
    HAL_TIM_PWM_ConfigChannel(&timer, &channel, RIGHT_CHANNEL);
    HAL_GPIO_Init(RIGHT_TIMER_GPIO, &gpio);

    HAL_TIM_PWM_Start(&timer, RIGHT_CHANNEL);
    rightMotorPWM.disable();
}

/**
 * initializes the timers and gpio for the pwm for the shooting bldc
 */
void HALManager::initializeShootingBLDC() {
    TIM_HandleTypeDef timer = { };
    TIM_OC_InitTypeDef channel = { };
    GPIO_InitTypeDef gpio = getDefaultGPIO();

    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    timer.Init.Prescaler = 80 - 1;
    timer.Init.Period = 1000 - 1;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Instance = BLDC_TIMER;

    channel.Pulse = 500;
    channel.OCMode = TIM_OCMODE_PWM1;
    channel.OCNPolarity = TIM_OCNPOLARITY_LOW; // Using CH1N!!

    gpio.Alternate = GPIO_AF3_TIM8;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = BLDC_TIMER_PIN;

    __HAL_RCC_TIM8_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    HAL_TIM_PWM_Init(&timer);
    HAL_TIM_PWM_ConfigChannel(&timer, &channel, BLDC_CHANNEL);
    HAL_GPIO_Init(BLDC_TIMER_GPIO, &gpio);

    HAL_TIM_PWM_Start(&timer, BLDC_CHANNEL);
    shootingBLDCPWM.disable();

    shootingBLDC.initialize();
}
#endif
/** @} */
