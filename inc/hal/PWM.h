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

class PWM {
private:
    volatile uint32_t *arr;
    volatile uint32_t *prescale;
    volatile uint32_t *ccr;
    float dutyCycle;
public:
    PWM(volatile uint32_t *arr, volatile uint32_t *prescale, volatile uint32_t *ccr);

    void setFrequency(int hz);
    void setPrescale(uint16_t prescale);
    void setDutyCycle(float percent);
};

#endif /* HAL_PWM_H_ */
/** @} */
