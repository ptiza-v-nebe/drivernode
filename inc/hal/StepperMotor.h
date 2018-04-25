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

#include "Motor.h"
#include "PWM.h"
#include "OutputPin.h"
#include <stm32l4xx.h>

class OutputPin;

class StepperMotor: public Motor {
private:
    PWM step; ///< use a hardware PWM to generate step signal
    OutputPin enablePin; ///< the enable pin
    OutputPin directionPin; ///< the direction pin
    bool enabled; ///< is the motor enabled
public:
    StepperMotor(PWM& step, OutputPin&& enablePin, OutputPin&& directionPin);

    // prevent copy and move
    StepperMotor(const StepperMotor&) = delete;
    StepperMotor(StepperMotor&&) = delete;
    StepperMotor& operator=(const StepperMotor&) = delete;
    StepperMotor& operator=(StepperMotor) = delete;

    void enable() override;
    void disableAndStop() override;

    void setSpeed(int32_t speed) override;
    void stop() override;

private:
    void setDirection(int direction);
};

#endif /* HAL_STEPPERMOTOR_H_ */
/** @} */
