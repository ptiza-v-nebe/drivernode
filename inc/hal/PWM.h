/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_PWM_H_
#define HAL_PWM_H_

#include <cstdint>
#include <stm32l4xx.h>

class PWM {
private:
    TIM_TypeDef *timer; ///< the timer to be used
    volatile uint32_t *ccr; ///< the capture compare register to be used
    uint32_t enableMask; ///< the mask to be used for enabling and disabling the pwm
    float dutyCycle; ///< the duty cycle of the pwm signal
public:
    PWM(TIM_TypeDef *timer, uint32_t channel);

    void setFrequency(unsigned int hz);
    void setPrescale(uint16_t prescale);
    void setDutyCycle(float percent);

    void enable();
    void disable();
};

#endif /* HAL_PWM_H_ */
/** @} */
