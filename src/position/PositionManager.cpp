/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#include <position/PositionManager.h>
#include "position/calibration_parameters.h"
#include <cmath>

PositionManager::PositionManager(Encoder& left, Encoder& right) :
        xum(0), yum(0), heading(0), left(left), right(right) {
}

void PositionManager::reset(const Position& position, const Angle& heading) {
    left.reset();
    right.reset();
    xum = static_cast<float>(position.x) * 1000;
    yum = static_cast<float>(position.y) * 1000;
    this->heading = heading;
}

void PositionManager::update() {
    // read encoders
    int ticksLeft = left.getTickAndReset();
    int ticksRight = right.getTickAndReset();

    // convert ticks to micrometers (um)
    float umLeft = ticksLeft * UM_PER_TICK_L;
    float umRight = ticksRight * UM_PER_TICK_R;

    // general formula for changes in angle
    float delta_alpha = (umRight - umLeft) / TRACK_WIDTH_UM;

    // distance traveled
    float delta_s = (umLeft + umRight) / 2;

    // both motors traveled the same distance
    if (std::abs(delta_alpha) < EPSILON) {
        xum += delta_s * heading.cos();
        yum += delta_s * heading.sin();
    }

    // robot is turning
    else {
        Angle newHeading = heading + delta_alpha;
        xum += (delta_s / delta_alpha)
                * (newHeading.sin() - heading.sin());
        yum += (delta_s / delta_alpha)
                * (-newHeading.cos() + heading.cos());
        heading = newHeading;
    }
}

Position PositionManager::getPosition() const {
    return {static_cast<uint16_t>(xum / 1000), static_cast<uint16_t>(yum / 1000)};
}

const Angle& PositionManager::getHeading() const {
    return heading;
}
/** @} */
