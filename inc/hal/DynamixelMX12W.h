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

#include "hal/Motor.h"
#include "hal/DynamixelCOM.h"

class DynamixelMX12W: public Motor {
private:
    const uint8_t id; ///< the ID of the servo
    bool invert; ///< used to invert the direction
    bool enabled; ///< is the servo enabled
    DynamixelCOM& com; ///< reference to the COM object
public:
    DynamixelMX12W(uint8_t id, DynamixelCOM& com, bool invet = false);

    // prevent copy and move
    DynamixelMX12W(const DynamixelMX12W&) = delete;
    DynamixelMX12W(DynamixelMX12W&&) = delete;
    DynamixelMX12W& operator=(const DynamixelMX12W&) = delete;
    DynamixelMX12W& operator=(DynamixelMX12W&&) = delete;

    virtual void enable() override;
    virtual void disableAndStop() override;

    void setRPM(float rpm);
    void setSpeed(int16_t speed) override;

    void stop() override;
};

#endif /* HAL_DYNAMIXELMX12W_H_ */
/** @} */
