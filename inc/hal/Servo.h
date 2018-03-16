/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */


#ifndef HAL_SERVO_H_
#define HAL_SERVO_H_

#include "hal/Actor.h"

class Servo : public Actor {
public:

    virtual ~Servo() = default;

    /**
     * @return the current angle in radians
     */
    virtual float getAngle() = 0;

    /**
     * Move to a specific angle
     *
     * @param angle the angle to move to in radians
     */
    virtual void moveTo(float angle) = 0;

    /**
     * Set movement speed.
     *
     * @param rpm the target revolutions per minute
     */
    virtual void setRPM(float rpm) = 0;
};



#endif /* HAL_SERVO_H_ */
/** @} */
