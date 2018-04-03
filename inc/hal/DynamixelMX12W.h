/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_DYNAMIXELMX12W_H_
#define HAL_DYNAMIXELMX12W_H_

#include "hal/Actor.h"
#include "hal/DynamixelCOM.h"

class DynamixelMX12W : public Actor {
private:
    const uint8_t id; ///< the ID of the servo
    int directionSign; ///< used to invert the direction
    bool enabled; ///< is the servo enabled
    DynamixelCOM& com; ///< reference to the COM object
public:
    DynamixelMX12W(uint8_t id, DynamixelCOM& com, int directionSign = 1);

    virtual void enable() override;
    virtual void disableAndStop() override;

    void setRPM(float rpm);
};

#endif /* HAL_DYNAMIXELMX12W_H_ */
/** @} */
