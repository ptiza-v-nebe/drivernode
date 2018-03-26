/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/FaulhaberBLDC.h>
#include "hal/util.h"

static constexpr uint16_t MAX_SPEED = 4095;
#define MOTOR_GPIO GPIOA
#define MOTOR_DAC DAC1

FaulhaberBLDC::FaulhaberBLDC(uint8_t dac_channel, uint16_t dac_pin,
        GPIO_TypeDef* direction_gpio, uint16_t direction_pin,
        bool reverseDirection) :
        enabled(true), dac_channel(dac_channel), dac_pin(dac_pin), direction_gpio(
                direction_gpio), direction_pin(direction_pin), reverseDirection(
                reverseDirection) {
    init();
    disableAndStop();
}

/*
 * @see - Actor::enable()
 */
void FaulhaberBLDC::enable() {
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void FaulhaberBLDC::disableAndStop() {
    stop();
    enabled = false;
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void FaulhaberBLDC::setSpeed(uint16_t speed) {
    if (!enabled) {
        return;
    }

    if (speed > MAX_SPEED) {
        setSpeed(MAX_SPEED);
    } else {
        HAL_DAC_SetValue(&dac, dac_channel, DAC_ALIGN_12B_R, speed);
    }
}

/*
 * @see - Motor:setDirection(DriveDirection)
 */
void FaulhaberBLDC::setDirection(DriveDirection direction) {
    GPIO_PinState pinState;
    if (reverseDirection) {
        switch (direction) {
            case DriveDirection::FORWARD:
                pinState = GPIO_PIN_SET;
                break;
            default:
                pinState = GPIO_PIN_RESET;
        }
    } else {
        switch (direction) {
            case DriveDirection::BACKWARD:
                pinState = GPIO_PIN_SET;
                break;
            default:
                pinState = GPIO_PIN_RESET;
        }
    }

    HAL_GPIO_WritePin(direction_gpio, direction_pin, pinState);
}

/*
 * @see - Motor::stop()
 */
void FaulhaberBLDC::stop() {
    setSpeed(0);
}

/**
 * Initializes the underlying CubeHAL
 */
void FaulhaberBLDC::init() {
    // init DAC
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_DAC1_CLK_ENABLE();
    dac.Instance = MOTOR_DAC;

    GPIO_InitTypeDef gpio_dac = getDefaultGPIO();
    gpio_dac.Mode = GPIO_MODE_ANALOG;
    gpio_dac.Pin = dac_pin;

    HAL_DAC_Init(&dac);
    HAL_GPIO_Init(MOTOR_GPIO, &gpio_dac);
    HAL_DAC_Start(&dac, dac_channel);

    // init direction
    GPIO_InitTypeDef gpio_dir = getDefaultGPIO();
    gpio_dir.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_dir.Pin = direction_pin;

    HAL_GPIO_Init(direction_gpio, &gpio_dir);
}
/** @} */
