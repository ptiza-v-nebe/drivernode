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

    Encoder& left;
    Encoder& right;
public:
    PositionManager(Encoder& left, Encoder& right);


    void reset(const Position& position, const Angle& heading);
    void tick() override;

    Position getPosition() const;
    const Angle& getHeading() const;
};

#endif /* POSITION_POSITIONMANAGER_H_ */
/** @} */
