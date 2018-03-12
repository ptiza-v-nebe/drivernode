/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#include <position/PositionManager.h>

PositionManager::PositionManager(Encoder& left, Encoder& right) :
        xum(0), yum(0), heading(0), left(left), right(right) {
}

void PositionManager::reset(uint16_t x, uint16_t y, float heading) {
    left.reset();
    right.reset();
    xum = x * 1000;
    yum = y * 1000;
    this->heading = heading;
}

void PositionManager::update() {
    int ticksLeft = left.getTickAndReset();
    int ticksRight = right.getTickAndReset();

    // TODO: calculation magic
}

uint16_t PositionManager::getX() {
    return static_cast<uint16_t>(xum / 1000);
}

uint16_t PositionManager::getY() {
    return static_cast<uint16_t>(yum / 1000);
}

float PositionManager::getHeading() {
    return heading;
}
/** @} */
