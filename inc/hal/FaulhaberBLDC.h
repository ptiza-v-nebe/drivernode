/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_FAULHABERBLDC_H_
#define HAL_FAULHABERBLDC_H_

#include <hal/Motor.h>
#include <stm32l4xx.h>

class FaulhaberBLDC: public Motor {
private:
    bool enabled; ///< is the motor enabled
    DAC_HandleTypeDef dac; ///< handle for accessing the dac
    uint8_t dac_channel; ///< which DAC channel to use
    uint16_t dac_pin; ///< which GPIO Pin to use
    GPIO_TypeDef *direction_gpio; ///< the GPIO bank used for direction output
    uint16_t direction_pin; ///< the pin used for direction control
    bool reverseDirection; ///< should the direction pin be inverted
public:
    FaulhaberBLDC(uint8_t dac_channel, uint16_t dac_pin,
            GPIO_TypeDef *direction_gpio, uint16_t direction_pin,
            bool reverseDirection = false);

    virtual void enable() override;
    virtual void disableAndStop() override;
    virtual void setSpeed(uint16_t speed) override;
    virtual void setDirection(DriveDirection direction) override;
    virtual void stop() override;
private:
    void init();
};

#endif /* HAL_FAULHABERBLDC_H_ */
/** @} */
