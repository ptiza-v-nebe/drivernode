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

PWM::PWM(TIM_HandleTypeDef* timer, uint32_t channel) :
        timer(timer), ocHandle { 0 }, channel(channel), dutyCycle(0.5f) {
            init();
}

void PWM::setFrequency(int hz) {
    float frequency = static_cast<float>(HAL_RCC_GetSysClockFreq()) / (timer->Init.Prescaler + 1);
    uint32_t divider = static_cast<uint32_t>(frequency / hz);
    timer->Init.Period = divider - 1;
    setDutyCycle(dutyCycle);
}

void PWM::setPrescale(uint16_t prescale) {
    timer->Init.Prescaler = prescale - 1; // TODO: check if this is enough
}

void PWM::setDutyCycle(float percent) {
    if(percent > 1 || percent < 0) {
        return;
    }
    dutyCycle = percent;
    ocHandle.Pulse = std::round(timer->Init.Period * dutyCycle);
    HAL_TIM_PWM_ConfigChannel(timer, &ocHandle, channel);
}

void PWM::enable() {
    HAL_TIM_PWM_Start(timer, channel);
}

void PWM::disable() {
    HAL_TIM_PWM_Stop(timer, channel);
}

void PWM::init() {
    ocHandle.OCMode = TIM_OCMODE_PWM1;
    setDutyCycle(dutyCycle);
}
/** @} */
