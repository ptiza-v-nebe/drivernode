/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/OutputPin.h>
#include <hal/util.h>

/**
 * Constructs an OutputPin.
 *
 * @param gpio    the GPIO bank
 * @param pin     the GPIO pin
 * @param onState the state of the pin to turn the connected device on (default GPIO_PIN_SET)
 * @param pullUp  the pull-up/pull-down configuration (default GPIO_NOPULL)
 * @param mode    the mode of the pin (default GPIO_MODE_OUTPUT_PP)
 */
OutputPin::OutputPin(GPIO_TypeDef* gpio, uint16_t pin, GPIO_PinState onState,
        uint32_t pullUp, uint32_t mode) :
        gpio(gpio), pin(pin), enabled(true), onState(onState) {
    init(pullUp, mode);
    disableAndStop();
}

/*
 * @see - Actor::enable()
 */
void OutputPin::enable() {
    enabled = true;
}

/*
 * @see - Actor::disable()
 */
void OutputPin::disableAndStop() {
    setOff();
    enabled = false;
}

/**
 * Sets the pin.
 *
 * @param on whether or not the pin should be turned on
 */
void OutputPin::set(bool on) {
    if (!enabled) {
        return;
    }
    HAL_GPIO_WritePin(gpio, pin,
            (on ? onState : static_cast<GPIO_PinState>(!onState)));
}

/**
 * turn the pin on
 */
void OutputPin::setOn() {
    set(true);
}

/**
 * turn the pin off
 */
void OutputPin::setOff() {
    set(false);
}

void OutputPin::toggle() {
    HAL_GPIO_TogglePin(gpio, pin);
}

/**
 * Initializes the GPIO pin
 *
 * @param pullUp the pull-up/pull-down configuration
 * @param mode   the output mode
 */
void OutputPin::init(uint32_t pullUp, uint32_t mode) {
    GPIO_InitTypeDef gpioInit = getDefaultGPIO();
    gpioInit.Pin = pin;
    gpioInit.Pull = pullUp;
    gpioInit.Mode = mode;

    HAL_GPIO_Init(gpio, &gpioInit);
}
/** @} */
