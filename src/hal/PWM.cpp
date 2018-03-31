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

PWM::PWM(TIM_TypeDef* timer, uint32_t channel) :
        timer(timer) {
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
            // TODO: error handling
            while(1) {}
    }

    setDutyCycle(0.5);
}

void PWM::setFrequency(unsigned int hz) {
    float frequency = static_cast<float>(HAL_RCC_GetSysClockFreq())
            / (timer->PSC + 1);
    uint32_t divider = static_cast<uint32_t>(frequency / hz);
    timer->ARR = divider - 1;
    setDutyCycle(dutyCycle);
}

void PWM::setPrescale(uint16_t prescale) {
    timer->PSC = prescale - 1;
}

void PWM::setDutyCycle(float percent) {
    if (percent > 1 || percent < 0) {
        return;
    }
    dutyCycle = percent;
    *ccr = std::round(timer->ARR * dutyCycle);
}

void PWM::enable() {
    SET_BIT(timer->CCER, enableMask);
}

void PWM::disable() {
    CLEAR_BIT(timer->CCER, enableMask);
}
/** @} */
