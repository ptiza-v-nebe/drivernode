/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/ShootingBLDC.h>

/**
 * prescaler for the pwm
 */
static constexpr int PRESCALE = 80; // timer base frequency = 80MHz / 80 = 1 MHz

/**
 * frequency for the pwm
 */
static constexpr int FREQUENCY = 50; // ESC uses 50 Hz

/**
 * duty cycle percentage for stopping the motor
 */
static constexpr float STOP_PERCENT = 0.05f;

/**
 * duty cycle percentage for running the motor
 */
static constexpr float ON_PERCENT = 0.0575f;

/**
 * Constructs a ShootingBLDC
 *
 * @param pwm reference to the PWM
 */
ShootingBLDC::ShootingBLDC(PWM& pwm) :
        pwm(pwm), enabled(false) {
    pwm.setPrescale(PRESCALE);
    pwm.setFrequency(FREQUENCY);

    pwm.setDutyCycle(STOP_PERCENT);
}

/*
 * @see - Actor::enable()
 */
void ShootingBLDC::enable() {
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void ShootingBLDC::disableAndStop() {
    stop();
    enabled = false;
}

/**
 * starts the motor
 */
void ShootingBLDC::start() {
    if (!enabled) {
        return;
    }
    pwm.setDutyCycle(ON_PERCENT);
}

/**
 * stops the motor
 */
void ShootingBLDC::stop() {
    if (!enabled) {
        return;
    }
    pwm.setDutyCycle(STOP_PERCENT);
}
/** @} */
