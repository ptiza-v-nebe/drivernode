/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/PWM.h>
#include <cmath>
#include <stm32l4xx.h>
#include "error.h"

/**
 * Constructs a PWM abstraction.
 *
 * @param timer   the timer to be used
 * @param channel the channel to be used
 */
PWM::PWM(TIM_TypeDef* timer, uint32_t channel) :
        timer(timer), ccr(nullptr), enableMask(0), dutyCycle(0.5) {
    switch (channel) {
        case TIM_CHANNEL_1:
            ccr = &(timer->CCR1);
            enableMask = TIM_CCER_CC1E | TIM_CCER_CC1NE;
            break;
        case TIM_CHANNEL_2:
            ccr = &(timer->CCR2);
            enableMask = TIM_CCER_CC2E | TIM_CCER_CC2NE;
            break;
        case TIM_CHANNEL_3:
            ccr = &(timer->CCR3);
            enableMask = TIM_CCER_CC3E | TIM_CCER_CC3NE;
            break;
        case TIM_CHANNEL_4:
            ccr = &(timer->CCR4);
            enableMask = TIM_CCER_CC4E;
            break;
        default:
            ERROR("Invalid Channel!");
    }

    setDutyCycle(0.5);
}

/**
 * Sets the PWM frequency.
 * @attention this affects ALL PWM signals that share this timer!!
 *
 * @param hz the frequency in hz
 */
void PWM::setFrequency(unsigned int hz) {
    if(hz == 0){
        ERROR("0 Hz is not a valid frequency!");
        return;
    }

    float frequency = static_cast<float>(HAL_RCC_GetSysClockFreq())
            / (timer->PSC + 1);
    uint32_t divider = static_cast<uint32_t>(frequency / hz);

    if(divider == 0) {
        ERROR("Frequency is too big for current prescale value!");
        return;
    }
    timer->ARR = divider - 1;
    setDutyCycle(dutyCycle);
}

/**
 * Set the prescaler.
 * @attention This affects frequency of all PWM signals that share this timer!!
 *
 * @param prescale the new prescale
 */
void PWM::setPrescale(uint16_t prescale) {
    timer->PSC = prescale - 1;
}

/**
 * Sets the duty cycle of the PWM signal in percent.
 *
 * @param percent the duty cycle in percent (between 0 and 1)
 */
void PWM::setDutyCycle(float percent) {
    if (percent > 1 || percent < 0) {
        return;
    }
    dutyCycle = percent;
    *ccr = std::round(timer->ARR * dutyCycle);
}

/**
 * enable the PWM signal
 */
void PWM::enable() {
    SET_BIT(timer->CCER, enableMask);
}

/**
 * disable the PWM signal
 */
void PWM::disable() {
    CLEAR_BIT(timer->CCER, enableMask);
}
/** @} */
