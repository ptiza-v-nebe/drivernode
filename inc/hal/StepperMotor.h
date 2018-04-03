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
    PWM step; ///< use a hardware PWM to generate step signal
    uint16_t directionPin; ///< the direction pin
    bool invertDirection; ///< should the direction pin be inverted
    uint16_t enablePin; ///< the enable pin
    bool invertEnable; ///< should the enable pin be inverted
    GPIO_TypeDef *gpio; ///< the gpio bank to use
    bool enabled; ///< is the motor enabled
public:
    StepperMotor(PWM& step, GPIO_TypeDef *gpio, uint16_t directionPin,
            uint16_t enablePin, bool invertDirection = false,
            bool invertEnable = false);

    // prevent copy and move
    StepperMotor(const StepperMotor&) = delete;
    StepperMotor(StepperMotor&&) = delete;
    StepperMotor& operator=(const StepperMotor&) = delete;
    StepperMotor& operator=(StepperMotor) = delete;

    void enable() override;
    void disableAndStop() override;

    void setSpeed(int16_t speed) override;
    void stop() override;

private:
    void setDirection(int direction);
};

#endif /* HAL_STEPPERMOTOR_H_ */
/** @} */
