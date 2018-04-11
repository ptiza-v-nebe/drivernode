/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/InputPin.h>
#include <hal/util.h>

/**
 * Constructs an InputPin
 *
 * @param gpio    the GPIO bank
 * @param pin     the GPIO pin
 * @param onState the GPIO_PinState when the connected device is on (default GPIO_PIN_SET)
 * @param pullUp  the pull-up / pull-down configuration (default GPIO_NOPULL)
 */
InputPin::InputPin(GPIO_TypeDef* gpio, uint16_t pin, GPIO_PinState onState,
        uint32_t pullUp) :
        gpio(gpio), pin(pin), onState(onState) {
    init(pullUp);
}


bool InputPin::isOn() {
    return HAL_GPIO_ReadPin(gpio, pin) == onState;
}

bool InputPin::isOff() {
    return !isOn();
}

void InputPin::init(uint32_t pullUp) {
    GPIO_InitTypeDef gpioInit = getDefaultGPIO();
    gpioInit.Pin = pin;
    gpioInit.Pull = pullUp;

    HAL_GPIO_Init(gpio, &gpioInit);
}
/** @} */
