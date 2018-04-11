/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_INPUTPIN_H_
#define HAL_INPUTPIN_H_

#include <stm32l4xx.h>

class InputPin {
private:
    GPIO_TypeDef *gpio; ///< the GPIO bank to be used
    uint16_t pin; ///< the pin to be used
    GPIO_PinState onState; ///< the pin state when the connected device is on
public:
    InputPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState onState =
            GPIO_PIN_SET, uint32_t pullUp = GPIO_NOPULL);

    // prevent copy and move
    InputPin(const InputPin&) = delete;
    InputPin(InputPin&&) = delete;
    InputPin& operator=(const InputPin&) = delete;
    InputPin& operator=(InputPin&&) = delete;

    bool isOn();
    bool isOff();
private:
    void init(uint32_t pullUp);
};

#endif /* HAL_INPUTPIN_H_ */
/** @} */
