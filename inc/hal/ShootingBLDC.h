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
    PWM& pwm; ///< the pwm used for output
    bool enabled; ///< is the actor enabled
public:
    ShootingBLDC(PWM& pwm);
    virtual ~ShootingBLDC() = default;

    // prevent copy and move
    ShootingBLDC(const ShootingBLDC&) = delete;
    ShootingBLDC(ShootingBLDC&&) = delete;
    ShootingBLDC& operator=(const ShootingBLDC&) = delete;
    ShootingBLDC& operator=(ShootingBLDC&&) = delete;

    void enable() override;
    void disableAndStop() override;

    void start();
    void stop();

    void initialize();
};

#endif /* HAL_SHOOTINGBLDC_H_ */
/** @} */
