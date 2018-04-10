/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelAX12A.h>
#include "error.h"

/**
 * address of the enable register
 */
static constexpr uint8_t ENABLE_ADDR = 24;

/**
 * address of the present position register
 */
static constexpr uint8_t PRESENT_POSITION_ADDR = 36;

/**
 * address of the goal position register
 */
static constexpr uint8_t GOAL_POSITION_ADDR = 30;

/**
 * address of the moving speed register
 */
static constexpr uint8_t MOVING_SPEED_ADDR = 32;

/**
 * maximum valid position
 */
static constexpr uint16_t MAX_POSITION = 1023;

/**
 * conversion from position to angle in degrees
 */
static constexpr float POSITION_TO_DEGREES = 0.29;

/**
 * maximum speed
 */
static constexpr uint16_t MAX_SPEED = 1023;

/**
 * conversion from speed to rpm
 */
static constexpr float SPEED_TO_RPM = 0.111;

/**
 * Constructs an abstraction for the Dynamxiel AX12A servo
 * @attention DynamixelAX12A is DISABLED by default.
 *
 * @param id  the id of the servo
 * @param com reference to the com object for sending commands
 */
DynamixelAX12A::DynamixelAX12A(uint8_t id, DynamixelCOM& com) :
        id(id), enabled(false), com(com) {
    disableAndStop();
}

/*
 * @see - Actor::enable()
 */
void DynamixelAX12A::enable() {
    uint8_t result = com.writeByte(id, ENABLE_ADDR, 1);
    if (result) {
        ERROR("Failed to enable Dynamixel %d - Result is %x", id, result);
    }
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void DynamixelAX12A::disableAndStop() {
    uint8_t result = com.writeByte(id, ENABLE_ADDR, 0);
    if (result) {
        ERROR("Failed to disable Dynamixel %d - Result is %x", id, result);
    }
    enabled = false;
}

/*
 * @see - Servo::getAngle()
 */
Angle DynamixelAX12A::getAngle() {
    uint16_t position;
    uint8_t result = com.readWord(id, PRESENT_POSITION_ADDR, position);
    if (result) {
        ERROR("Failed to read angle from Dynamixel %d - Result is %x", id,
                result);
    }

    return Angle::getFromDegrees(POSITION_TO_DEGREES * position);
}

/*
 * @see - Servo::moveTo
 */
void DynamixelAX12A::moveTo(const Angle& angle) {
    if (!enabled) {
        return;
    }
    uint16_t position = static_cast<uint16_t>(angle.getAngleInDegrees()
            / POSITION_TO_DEGREES);
    if (position > MAX_POSITION) {
        position = MAX_POSITION;
    }
    uint8_t result = com.writeWord(id, GOAL_POSITION_ADDR, position);
    if (result) {
        ERROR("Failed to send angle to Dynamixel %d - Result is %x", id, result);
    }
}

void DynamixelAX12A::setRPM(float rpm) {
    uint16_t speed = static_cast<uint16_t>(rpm / SPEED_TO_RPM);
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    uint8_t result = com.writeWord(id, MOVING_SPEED_ADDR, speed);

    if (result) {
        ERROR("Failed to send speed to Dynamixel %d - Result is %x", id, result);
    }
}
/** @} */
