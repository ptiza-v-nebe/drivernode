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

PWM::PWM(volatile uint32_t* arr, volatile uint32_t* prescale,
        volatile uint32_t* ccr) : arr(arr), prescale(prescale), ccr(ccr), dutyCycle(0.5) {
    setDutyCycle(dutyCycle);
}


void PWM::setFrequency(int hz) {
    float frequency = static_cast<float>(HAL_RCC_GetSysClockFreq()) / (*prescale + 1);
    uint32_t divider = static_cast<uint32_t>(frequency / hz);
    *arr = divider - 1;
    setDutyCycle(dutyCycle);
}

void PWM::setPrescale(uint16_t prescale) {
    *(this->prescale) = prescale - 1;
}

void PWM::setDutyCycle(float percent) {
    if(percent > 1 || percent < 0) {
        return;
    }
    dutyCycle = percent;
    *ccr = std::round(*arr * dutyCycle);
}
/** @} */
