/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <OutputPin.h>
#include <hal/util.h>

OutputPin::OutputPin(GPIO_TypeDef* gpio, uint16_t pin, GPIO_PinState onState,
        uint32_t pullUp, uint32_t mode) :
        gpio(gpio), pin(pin), enabled(true), onState(onState) {
    init(pullUp, mode);
    disableAndStop();
}

void OutputPin::enable() {
    enabled = true;
}

void OutputPin::disableAndStop() {
    setOff();
    enabled = false;
}

void OutputPin::set(bool on) {
    if (!enabled) {
        return;
    }
    HAL_GPIO_WritePin(gpio, pin, (on ? onState : static_cast<GPIO_PinState>(!onState)));
}

void OutputPin::setOn() {
    set(true);
}

void OutputPin::setOff() {
    set(false);
}

void OutputPin::init(uint32_t pullUp, uint32_t mode) {
    GPIO_InitTypeDef gpioInit = getDefaultGPIO();
    gpioInit.Pin = pin;
    gpioInit.Pull = pullUp;
    gpioInit.Mode = mode;

    HAL_GPIO_Init(gpio, &gpioInit);
}
/** @} */
