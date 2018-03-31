/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/StepperMotor.h>

StepperMotor::StepperMotor(PWM step, GPIO_TypeDef* gpio, uint16_t directionPin,
        uint16_t enablePin, bool invertDirection, bool invertEnable) :
        step(step), directionPin(directionPin), invertDirection(
                invertDirection), enablePin(enablePin), invertEnable(
                invertEnable), gpio(gpio), enabled(true) {
    disableAndStop();
}

void StepperMotor::enable() {
    enabled = true;
    HAL_GPIO_WritePin(gpio, enablePin,
            (invertEnable ? GPIO_PIN_RESET : GPIO_PIN_SET));
}

void StepperMotor::disableAndStop() {
    enabled = false;
    stop();
}

void StepperMotor::setSpeed(int16_t speed) {
    if (!enabled) {
        return;
    }

    if (speed == 0) {
        stop();
    } else {
        enable();
    }

    if (speed < 0) {
        setDirection(-1);
        step.setFrequency(-speed);
    } else {
        setDirection(1);
        step.setFrequency(speed);
    }
}

void StepperMotor::stop() {
    HAL_GPIO_WritePin(gpio, enablePin,
            (invertEnable ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void StepperMotor::setDirection(int direction) {
    if (direction < 0) {
        HAL_GPIO_WritePin(gpio, directionPin,
                (invertDirection ? GPIO_PIN_RESET : GPIO_PIN_SET));
    } else {
        HAL_GPIO_WritePin(gpio, directionPin,
                (invertDirection ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }
}
/** @} */
