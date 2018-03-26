/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */


#ifndef HAL_MOTOR_H_
#define HAL_MOTOR_H_

#include "hal/Actor.h"
#include "serial/messages/drive_types.h"

#include <cstdint>

class Motor : public Actor {
public:
    virtual ~Motor() = default;

    /**
     * Set the speed of the motor.
     *
     * @param speed the speed to set
     */
    virtual void setSpeed(int16_t speed) = 0;

    /**
     * Stop the motor
     */
    virtual void stop() = 0;
};



#endif /* HAL_MOTOR_H_ */
/** @} */
