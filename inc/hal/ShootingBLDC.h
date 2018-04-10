/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_SHOOTINGBLDC_H_
#define HAL_SHOOTINGBLDC_H_

#include <Actor.h>
#include <PWM.h>

class ShootingBLDC: public Actor {
private:
    PWM& pwm;
public:
    ShootingBLDC(PWM& pwm);
    virtual ~ShootingBLDC() = default;

    void enable() override;
    void disableAndStop() override;
};

#endif /* HAL_SHOOTINGBLDC_H_ */
/** @} */
