/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/StepperMotor.h>

/**
 * Constructs a StepperMotor abstraction.
 * @attention StepperMotor is DISABLED by default!
 *
 * @param step            the PWM used to generate step signal
 * @param gpio            the gpio bank
 * @param directionPin    the direction pin
 * @param enablePin       the enable pin
 * @param invertDirection should the direction pin be inverted?
 * @param invertEnable    should the enable pin be inverted?
 */
StepperMotor::StepperMotor(PWM& step, GPIO_TypeDef* gpio, uint16_t directionPin,
        uint16_t enablePin, bool invertDirection, bool invertEnable) :
        step(step), directionPin(directionPin), invertDirection(
                invertDirection), enablePin(enablePin), invertEnable(
                invertEnable), gpio(gpio), enabled(true) {
}

/*
 * @see - Actor::enable()
 */
void StepperMotor::enable() {
    enabled = true;
    HAL_GPIO_WritePin(gpio, enablePin,
            (invertEnable ? GPIO_PIN_RESET : GPIO_PIN_SET));
}

/*
 * @see - Actor::disableAndStop()
 */
void StepperMotor::disableAndStop() {
    enabled = false;
    stop();
    HAL_GPIO_WritePin(gpio, enablePin,
            (invertEnable ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void StepperMotor::setSpeed(int16_t speed) {
    if (!enabled) {
        return;
    }

    if (speed == 0) {
        step.disable();
    } else {
        step.enable();
    }

    if (speed < 0) {
        setDirection(-1);
        step.setFrequency(-speed);
    } else {
        setDirection(1);
        step.setFrequency(speed);
    }
}

/*
 * @see - Motor::stop()
 */
void StepperMotor::stop() {
    setSpeed(0);
}

/**
 * sets the direction.
 *
 * @param direction positive means forward, negative means backward
 */
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
