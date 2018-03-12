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

class PositionManager {
private:
    float xum;
    float yum;
    float heading;

    Encoder& left;
    Encoder& right;
public:
    PositionManager(Encoder& left, Encoder& right);


    void reset(uint16_t x, uint16_t y, float heading);
    void update();

    uint16_t getX();
    uint16_t getY();
    float getHeading();
};

#endif /* POSITION_POSITIONMANAGER_H_ */
/** @} */
