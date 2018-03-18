/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelAX12A.h>

static constexpr uint8_t ENABLE_ADDR = 24;
static constexpr uint8_t PRESENT_POSITION_ADDR = 36;
static constexpr uint8_t GOAL_POSITION_ADDR = 30;
static constexpr uint8_t MOVING_SPEED_ADDR = 32;

static constexpr uint16_t MAX_POSITION = 1023;
static constexpr float POSITION_TO_DEGREES = 0.29;

static constexpr uint16_t MAX_SPEED = 1023;
static constexpr float SPEED_TO_RPM = 0.111;

DynamixelAX12A::DynamixelAX12A(uint8_t id, DynamixelCOM& com) :
        id(id), enabled(false), com(com) {
    enable();
}

void DynamixelAX12A::enable() {
    com.writeByte(id, ENABLE_ADDR, 1); // TODO: error handling
    enabled = true;
}

void DynamixelAX12A::disableAndStop() {
    com.writeByte(id, ENABLE_ADDR, 0); // TODO: error handling
    enabled = false;
}

Angle DynamixelAX12A::getAngle() {
    uint16_t position;
    com.readWord(id, PRESENT_POSITION_ADDR, position); // TODO: error handling

    return Angle::getFromDegrees(POSITION_TO_DEGREES * position);
}

void DynamixelAX12A::moveTo(const Angle& angle) {
    if (!enabled) {
        return;
    }
    uint16_t position = static_cast<uint16_t>(angle.getAngleInDegrees() / POSITION_TO_DEGREES);
    if (position > MAX_POSITION) {
        return; // TODO: error handling
    }
    com.writeWord(id, GOAL_POSITION_ADDR, position); // TODO: error handling
}

void DynamixelAX12A::setRPM(float rpm) {
    uint16_t speed = static_cast<uint16_t>(rpm / SPEED_TO_RPM);
    if (speed > MAX_SPEED) {
        return; // TODO: error handling
    }
    com.writeWord(id, MOVING_SPEED_ADDR, speed); // TODO: error handling
}
/** @} */
