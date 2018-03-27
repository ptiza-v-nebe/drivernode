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

#include <cstdint>
#include "hal/Encoder.h"

#include "position/Angle.h"
#include "position/Position.h"

class PositionManager {
private:
    float xum;
    float yum;
    Angle heading;

    Encoder& left;
    Encoder& right;
public:
    PositionManager(Encoder& left, Encoder& right);


    void reset(const Position& position, const Angle& heading);
    void update();

    Position getPosition() const;
    const Angle& getHeading() const;
};

#endif /* POSITION_POSITIONMANAGER_H_ */
/** @} */
