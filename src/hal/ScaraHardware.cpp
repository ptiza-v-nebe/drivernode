/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup <Group Name>
 * @{
 ******************************************************************************
 */

#include <hal/ScaraHardware.h>
#include <hal/util.h>

// ///////////////////////////////////////////////////////////////////////////////
// Scara Config
// ///////////////////////////////////////////////////////////////////////////////
/**
 * ids for the dynamixels
 */
static constexpr uint8_t ARMSERVO_1_ID = 2;
static constexpr uint8_t ARMSERVO_2_ID = 5;
static constexpr uint8_t ARMSERVO_3_ID = 3;
static constexpr uint8_t ARMSERVO_4_ID = 7;

ScaraHardware::ScaraHardware(DynamixelCOM& dynamixelCOM) :
        motorPWM(TIM2, TIM_CHANNEL_1), //
        motor(motorPWM, GPIOH, GPIO_PIN_1, GPIO_PIN_0, true, true), // TODO: ugly magic values!!
        encoder(SCARA_ENCODER_GPIO, SCARA_ENCODER_A, SCARA_ENCODER_B,
                SCARA_ENCODER_INVERT), //
        endSwitch(GPIOB, GPIO_PIN_6), //
        pump(GPIOC, GPIO_PIN_3), //
        valve(GPIOC, GPIO_PIN_2), //
        armServos { { ARMSERVO_1_ID, dynamixelCOM }, //
                { ARMSERVO_2_ID, dynamixelCOM }, //
                { ARMSERVO_3_ID, dynamixelCOM }, //
                { ARMSERVO_4_ID, dynamixelCOM } }, //
        storagePumps { { GPIOC, GPIO_PIN_13 }, //
                { GPIOC, GPIO_PIN_14 }, //
                { GPIOC, GPIO_PIN_15 } } //
{
}

void ScaraHardware::initialize() {
    initializePWM();
    initializeMotor();
    initializeEncoder();
}

void ScaraHardware::disable() {
    motor.disableAndStop();
    pump.disableAndStop();
    for (int i = 0; i < STORAGE_PUMP_COUNT; i++) {
        storagePumps[i].disableAndStop();
    }
    for (int i = 0; i < DYNAMIXEL_COUNT; i++) {
        armServos[i].disableAndStop();
    }
}

void ScaraHardware::initializePWM() {
    TIM_HandleTypeDef timer = { };
    TIM_OC_InitTypeDef channel = { };
    GPIO_InitTypeDef gpio = getDefaultGPIO();

    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    timer.Init.Prescaler = 80 - 1;
    timer.Init.Period = 1000 - 1;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Instance = TIM2;

    channel.Pulse = 500;
    channel.OCMode = TIM_OCMODE_PWM1;

    gpio.Alternate = GPIO_AF1_TIM2;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_5;

    __HAL_RCC_TIM2_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    HAL_TIM_PWM_Init(&timer);
    HAL_TIM_PWM_ConfigChannel(&timer, &channel, TIM_CHANNEL_1);
    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1);
    motorPWM.disable();
}

void ScaraHardware::initializeMotor() {
    GPIO_InitTypeDef motorGPIO = getDefaultGPIO();
    motorGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    motorGPIO.Mode = GPIO_MODE_OUTPUT_PP;

    __HAL_RCC_GPIOH_CLK_ENABLE()
    ;

    HAL_GPIO_Init(GPIOH, &motorGPIO);
}

void ScaraHardware::initializeEncoder() {
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    GPIO_InitTypeDef gpio_encoder = getDefaultGPIO();
    gpio_encoder.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio_encoder.Pin = SCARA_ENCODER_A | SCARA_ENCODER_B;

    HAL_GPIO_Init(SCARA_ENCODER_GPIO, &gpio_encoder);
}

Motor& ScaraHardware::getLiftMotor() {
    return motor;
}

Encoder& ScaraHardware::getLiftEncoder() {
    return encoder;
}

InputPin& ScaraHardware::getEndSwitch() {
    return endSwitch;
}

OutputPin& ScaraHardware::getPump() {
    return pump;
}

OutputPin& ScaraHardware::getValve() {
    return valve;
}

OutputPin* ScaraHardware::getStoragePumps() {
    return storagePumps;
}

DynamixelAX12A* ScaraHardware::getArmServos() {
    return armServos;
}
/** @} */
