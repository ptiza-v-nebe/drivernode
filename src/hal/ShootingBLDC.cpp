/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/ShootingBLDC.h>

static constexpr int PRESCALE = 80; // timer base frequency = 80MHz / 80 = 1 MHz
static constexpr int FREQUENCY = 50; // ESC uses 50 Hz

static constexpr float STOP_PERCENT = 0.04f;
static constexpr float ON_PERCENT = 0.0575f;

ShootingBLDC::ShootingBLDC(PWM& pwm) :
        pwm(pwm) {
    pwm.setPrescale(PRESCALE);
    pwm.setFrequency(FREQUENCY);

    pwm.setDutyCycle(STOP_PERCENT);
}

void ShootingBLDC::enable() {
    pwm.setDutyCycle(ON_PERCENT);
}

void ShootingBLDC::disableAndStop() {
    pwm.setDutyCycle(STOP_PERCENT);
}
/** @} */
