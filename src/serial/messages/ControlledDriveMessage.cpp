/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/ControlledDriveMessage.h>
#include "serial/messages/util.h"
#include <cstdio>

/**
 * The size of the payload expected for this message
 */
static constexpr int PAYLOAD_SIZE = 5;

/**
 * Deserialize a ControlledDriveMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a ControlledDriveMessage-object if the buffer contains a valid message
 */
std::experimental::optional<ControlledDriveMessage> ControlledDriveMessage::deserialize(const uint8_t* msg,
        const int size) {
    if (size != PAYLOAD_SIZE) {
        // invalid message
        return std::experimental::nullopt;
    }

    if(msg[0] >= static_cast<uint8_t>(DriveSpeed::_Count)) {
        // invalid enum
        return std::experimental::nullopt;
    }
    DriveSpeed speed = static_cast<DriveSpeed>(msg[0]);
    int16_t x, y;
    serialToSystem(msg + 1, x);
    serialToSystem(msg + 3, y);

    return {ControlledDriveMessage(speed, {x, y})};
}

/**
 * Constructs a ControlledDriveMessage.
 *
 * @param speed the speed at which to drive
 * @param x     the target x position
 * @param y     the target y position
 */
ControlledDriveMessage::ControlledDriveMessage(const DriveSpeed& speed,
        const Position position) :
        Message(getMessageType()),
        speed(speed), position(position) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int ControlledDriveMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = static_cast<uint8_t>(speed);
    if (systemToSerial(position.x, buffer + 1, 2) < 0) {
        return -1;
    }
    if (systemToSerial(position.y, buffer + 3, 2) < 0) {
        return -1;
    }

    return PAYLOAD_SIZE;
}

/*
 * @see - Message::print()
 */
void ControlledDriveMessage::print() const {
    printf("ControlledDriveCommand[speed=%s, target=(%d, %d)]", enumToString(speed), position.x, position.y);
}

/**
 * @return the driving speed
 */
const DriveSpeed& ControlledDriveMessage::getSpeed() {
    return speed;
}

/**
 * @return the target position
 */
const Position& ControlledDriveMessage::getPosition() {
    return position;
}
/** @} */
