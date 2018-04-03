/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_STEPPERMOTOR_H_
#define HAL_STEPPERMOTOR_H_

#include "hal/Motor.h"
#include "hal/PWM.h"
#include <stm32l4xx.h>

class StepperMotor: public Motor {
private:
    PWM step;
    uint16_t directionPin;
    bool invertDirection;
    uint16_t enablePin;
    bool invertEnable;
    GPIO_TypeDef *gpio;
    bool enabled;
public:
    StepperMotor(PWM step, GPIO_TypeDef *gpio, uint16_t directionPin,
            uint16_t enablePin, bool invertDirection = false,
            bool invertEnable = false);

    void enable() override;
    void disableAndStop() override;

    void setSpeed(int16_t speed) override;
    void stop() override;

private:
    void setDirection(int direction);
};

#endif /* HAL_STEPPERMOTOR_H_ */
/** @} */
