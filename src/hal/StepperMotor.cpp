/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/StepperMotor.h>
#include <utility>
/**
 * Constructs a StepperMotor abstraction.
 * @attention StepperMotor is DISABLED by default!
 *
 * @param step      the PWM used to generate step signal
 * @param enable    the enable pin
 * @param direction the direction pin
 */
StepperMotor::StepperMotor(PWM& step, OutputPin&& enablePin, OutputPin&& directionPin) :
        step(step), enablePin(std::move(enablePin)), directionPin(std::move(directionPin)), enabled(false) {
    this->enablePin.enable();
    this->directionPin.enable();
}

/*
 * @see - Actor::enable()
 */
void StepperMotor::enable() {
    enabled = true;
    enablePin.setOn();
}

/*
 * @see - Actor::disableAndStop()
 */
void StepperMotor::disableAndStop() {
    stop();
    enabled = false;
    enablePin.setOff();
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void StepperMotor::setSpeed(int32_t speed) {
    if (!enabled) {
        return;
    }

    if (speed == 0) {
        step.disable();
        return;
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
        directionPin.setOn();
    } else {
        directionPin.setOff();
    }
}
/** @} */
