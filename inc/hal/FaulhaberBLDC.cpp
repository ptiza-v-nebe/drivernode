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

FaulhaberBLDC::FaulhaberBLDC(uint8_t channel, uint16_t pin) :
        enabled(false), channel(channel), pin(pin) {
    init();
    stop();
}

void FaulhaberBLDC::enable() {
    enabled = true;
}

void FaulhaberBLDC::disableAndStop() {
    stop();
    enabled = false;
}

void FaulhaberBLDC::setSpeed(uint16_t speed) {
    if(!enabled) {
        return;
    }

    if (speed > MAX_SPEED) {
        setSpeed(MAX_SPEED);
    } else {
        HAL_DAC_SetValue(&dac, channel, DAC_ALIGN_12B_R, speed);
    }
}

void FaulhaberBLDC::setDirection(DriveDirection) {
    // TODO: implement
}

void FaulhaberBLDC::stop() {
    setSpeed(0);
}

void FaulhaberBLDC::init() {
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_DAC1_CLK_ENABLE()
    ;
    dac.Instance = DAC1;

    GPIO_InitTypeDef gpio_dac = getDefaultGPIO();
    gpio_dac.Mode = GPIO_MODE_ANALOG;
    gpio_dac.Pin = pin;

    HAL_DAC_Init(&dac);
    HAL_GPIO_Init(GPIOA, &gpio_dac);
    HAL_DAC_Start(&dac, channel);
}
/** @} */
