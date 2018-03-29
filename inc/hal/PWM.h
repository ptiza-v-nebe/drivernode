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

#include <stm32l4xx.h>

class PWM {
private:
    TIM_HandleTypeDef *timer;
    TIM_OC_InitTypeDef ocHandle;
    uint32_t channel;
    float dutyCycle;
public:
    PWM(TIM_HandleTypeDef *timer, uint32_t channel);

    void setFrequency(int hz);
    void setPrescale(uint16_t prescale);
    void setDutyCycle(float percent);

    void enable();
    void disable();

private:
    void init();
};

#endif /* HAL_PWM_H_ */
/** @} */
