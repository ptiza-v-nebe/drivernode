/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#ifndef POSITION_POSITIONMANAGER_H_
#define POSITION_POSITIONMANAGER_H_

#include "Angle.h"
#include "Position.h"

#include "hal/Encoder.h"
#include <control/Clocked.h>
#include <cstdint>

class PositionManager : public Clocked {
private:
    float xum;
    float yum;
    Angle heading;

    float leftWheelVelocity;
    float rightWheelVelocity;
    float leftWheelDistance;
    float rightWheelDistance;

    float forwardVelocity;
    float rotationalVelocity;

    Encoder& left;
    Encoder& right;
public:
    PositionManager(Encoder& left, Encoder& right);
    friend class DriverFSM;

    void reset(const Position& position, const Angle& heading);
    void tick() override;

    Position getPosition() const;
    const Angle& getHeading() const;

    float getLeftWheelVelocity() const;
    float getRightWheelVelocity() const;
    float getForwardVelocity() const;
    float getRotationalVelocity() const;
};

#endif /* POSITION_POSITIONMANAGER_H_ */
/** @} */
