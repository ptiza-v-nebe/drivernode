/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/SimpleDriveMessage.h>

#include <cstdio>

/**
 * bitmask for the speed part of the payload
 */
static constexpr uint8_t SPEED_MASK = 0x0F;

/**
 * bitmask for the direction part of the payload
 */
static constexpr uint8_t DIRECTION_MASK = 0xF0;

/**
 * offset of the direction (in the payload)
 */
static constexpr int DIRECTION_OFFSET = 4;

/**
 * The size of the payload expected for this message
 */
static constexpr int PAYLOAD_SIZE = 1;

/**
 * Deserialize a SimpleDriveMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a SimpleDriveMessage-object if the buffer contains a valid message
 */
std::experimental::optional<SimpleDriveMessage> SimpleDriveMessage::deserialize(
        const uint8_t* msg, const int size) {
    if (size != PAYLOAD_SIZE) {
        // something is wrong
        return std::experimental::nullopt;
    } else {
        uint8_t spd = msg[0] & SPEED_MASK;
        uint8_t dir = (msg[0] & DIRECTION_MASK) >> DIRECTION_OFFSET;

        if(spd >= static_cast<uint8_t>(DriveSpeed::_Count) || dir >= static_cast<uint8_t>(DriveDirection::_Count)) {
            // invalid enum
            return std::experimental::nullopt;
        }

        DriveSpeed speed = static_cast<DriveSpeed>(spd);
        DriveDirection direction = static_cast<DriveDirection>(dir);
        return { {speed, direction} };
    }
}

/**
 * Constructs a SimpleDriveMessage.
 *
 * @param speed     the driving speed
 * @param direction the driving direction
 */
SimpleDriveMessage::SimpleDriveMessage(const DriveSpeed& speed,
        const DriveDirection& direction) :
        Message(getMessageType()), speed(speed), direction(direction) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int SimpleDriveMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        // cannot serialize because it doesn't fit
        return -1;
    }

    uint8_t payload = 0;
    payload |= static_cast<uint8_t>(speed) & SPEED_MASK;
    payload |= (static_cast<uint8_t>(direction) << DIRECTION_OFFSET)
            & DIRECTION_MASK;

    buffer[0] = payload;
    return PAYLOAD_SIZE;
}

/*
 * @see - Message::print()
 */
void SimpleDriveMessage::print() const {
    printf("SimpleDriveComand[speed=%s, direction=%s]", enumToString(speed),
            enumToString(direction));
}

/**
 * @return the driving speed
 */
const DriveSpeed& SimpleDriveMessage::getSpeed() {
    return speed;
}

/**
 * @return the driving direction
 */
const DriveDirection& SimpleDriveMessage::getDirection() {
    return direction;
}
/** @} */
