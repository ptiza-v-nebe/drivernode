/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelAX12A.h>
#include "util/conversions.h"

static constexpr uint8_t ENABLE_ADDR = 24;
static constexpr uint8_t PRESENT_POSITION_ADDR = 36;
static constexpr uint8_t GOAL_POSITION_ADDR = 30;
static constexpr uint8_t MOVING_SPEED_ADDR = 32;

static constexpr uint16_t MAX_POSITION = 1023;
static constexpr float POSITION_TO_DEGREES = 0.29;

static constexpr uint16_t MAX_SPEED = 1023;
static constexpr float SPEED_TO_RPM = 0.111;

Dynamixel_AX12A::Dynamixel_AX12A(uint8_t id, DynamixelCOM& com) :
        id(id), enabled(false), com(com) {
    enable();
}

void Dynamixel_AX12A::enable() {
    com.writeByte(id, ENABLE_ADDR, 1); // TODO: error handling
    enabled = true;
}

void Dynamixel_AX12A::disableAndStop() {
    com.writeByte(id, ENABLE_ADDR, 0); // TODO: error handling
    enabled = false;
}

float Dynamixel_AX12A::getPosition() {
    uint8_t buffer[2];
    com.read(id, PRESENT_POSITION_ADDR, 2, buffer); // TODO: error handling
    uint16_t position = buffer[0] | (buffer[1] << 8);

    return degreesToRadians(POSITION_TO_DEGREES * position);
}

void Dynamixel_AX12A::moveTo(float angle) {
    if (!enabled) {
        return;
    }

    float degrees = radiansToDegrees(angle);
    uint16_t position = static_cast<uint16_t>(degrees / POSITION_TO_DEGREES);
    if (position > MAX_POSITION) {
        return; // TODO: error handling
    }

    uint8_t buffer[2];
    buffer[0] = static_cast<uint8_t>(position);
    buffer[1] = static_cast<uint8_t>(position >> 8);

    com.write(id, GOAL_POSITION_ADDR, buffer, 2); // TODO: error handling
}

void Dynamixel_AX12A::setRPM(float rpm) {
    uint16_t speed = static_cast<uint16_t>(rpm / SPEED_TO_RPM);
    if (speed > MAX_SPEED) {
        return; // TODO: error handling
    }

    uint8_t buffer[2];
    buffer[0] = static_cast<uint8_t>(speed);
    buffer[1] = static_cast<uint8_t>(speed >> 8);

    com.write(id, MOVING_SPEED_ADDR, buffer, 2); // TODO: error handling
}
/** @} */
