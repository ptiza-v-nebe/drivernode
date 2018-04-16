/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_OUTPUTPIN_H_
#define HAL_OUTPUTPIN_H_

#include <Actor.h>
#include <stm32l4xx.h>

class OutputPin: public Actor {
private:
    GPIO_TypeDef *gpio; ///< the GPIO bank
    uint16_t pin; ///< the GPIO pin
    bool enabled; ///< is the actor enabled
    GPIO_PinState onState; ///< the default state to set when the device is on
public:
    OutputPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState onState = GPIO_PIN_SET, uint32_t pullUp = GPIO_NOPULL, uint32_t mode = GPIO_MODE_OUTPUT_PP);

    // prevent copy and move
    OutputPin(const OutputPin&) = delete;
    OutputPin(OutputPin&&) = default;
    OutputPin& operator=(const OutputPin&) = delete;
    OutputPin& operator=(OutputPin&&) = default;

    void enable() override;
    void disableAndStop() override;

    void set(bool on);
    void setOn();
    void setOff();
private:
    void init(uint32_t pullUp, uint32_t mode);
};

#endif /* HAL_OUTPUTPIN_H_ */
/** @} */
