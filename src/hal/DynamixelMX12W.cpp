/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelMX12W.h>
#include <cmath>
#include "error.h"

/**
 * address of the enable register
 */
static constexpr uint8_t ENABLE_ADDR = 24;

/**
 * address of the movement speed register
 */
static constexpr uint8_t MOVING_SPEED_ADDR = 32;

/**
 * maximum valid speed
 */
static constexpr uint16_t MAX_SPEED = 500; //1023 according to specification, but physical max speed is around 500

/**
 * offset for turning backwards
 */
static constexpr uint16_t CW_OFFSET = 1024;

/**
 * conversion factor from RPM to speed
 */
static constexpr float SPEED_TO_RPM = 0.916;

/**
 * Constructs an abstraction for a Dynamixel MX12W servo
 * @attention DynamixelMX12W is DISABLED by default.
 *
 * @param id     the ID of the servo
 * @param com    reference to the com object used to send commands
 * @param invert should the direction be inverted?
 */
DynamixelMX12W::DynamixelMX12W(uint8_t id, DynamixelCOM& com, bool invert) :
        id(id), invert(invert), enabled(false), com(com) {
    disableAndStop();
}

/*
 * @see - Actor::enable()
 */
void DynamixelMX12W::enable() {
    uint8_t result = com.writeByte(id, ENABLE_ADDR, 1);
    if (result) {
        ERROR("Failed to enable Dynamixel %d - Result is %x", id, result);
    }
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void DynamixelMX12W::disableAndStop() {
    uint8_t result = com.writeByte(id, ENABLE_ADDR, 0);
    if (result) {
        ERROR("Failed to disable Dynamixel %d - Result is %x", id, result);
    }
    enabled = false;
}

/**
 * Sets a specific RPM as movement speed.
 *
 * @param rpm the rpm to set
 */
void DynamixelMX12W::setRPM(float rpm) {
    if (!enabled) {
        return;
    }

    if (invert) {
        rpm *= -1;
    }
    setSpeed(static_cast<int16_t>(rpm / SPEED_TO_RPM));
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void DynamixelMX12W::setSpeed(int32_t speed) {
    if (!enabled) {
        return;
    }

    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    } else if (speed < -MAX_SPEED) {
        speed = -MAX_SPEED;
    }

    if (speed < 0) {
        speed += CW_OFFSET;
    }

    uint8_t result = com.writeWord(id, MOVING_SPEED_ADDR, speed);
    if (result) {
        ERROR("Failed to send speed to Dynamixel %d - Result is %x", id, result);
    }
}

/*
 * @see - Motor::stop()
 */
void DynamixelMX12W::stop() {
    setSpeed(0);
}
/** @} */
