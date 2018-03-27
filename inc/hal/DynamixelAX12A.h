/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_DYNAMIXELAX12A_H_
#define HAL_DYNAMIXELAX12A_H_

#include <hal/Servo.h>
#include "hal/DynamixelCOM.h"
#include <cstdint>

class DynamixelAX12A: public Servo {
private:
    const uint8_t id; ///< the ID of the servo
    bool enabled; ///< is the servo enabled
    DynamixelCOM& com; ///< reference to the COM object
public:
    DynamixelAX12A(uint8_t id, DynamixelCOM& com);

    virtual void enable() override;
    virtual void disableAndStop() override;

    virtual Angle getAngle() override;
    virtual void moveTo(const Angle& angle) override;
    virtual void setRPM(float rpm) override;
};

#endif /* HAL_DYNAMIXELAX12A_H_ */
/** @} */
