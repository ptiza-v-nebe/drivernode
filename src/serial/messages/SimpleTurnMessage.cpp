/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/SimpleTurnMessage.h>

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
 * Deserialize a SimpleTurnMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a SimpleTurnMessage-object if the buffer contains a valid message
 */
std::experimental::optional<SimpleTurnMessage> SimpleTurnMessage::deserialize(
        const uint8_t* msg, const int size) {
    if (size != PAYLOAD_SIZE) {
        // invalid
        return std::experimental::nullopt;
    }

    uint8_t spd = msg[0] & SPEED_MASK;
    uint8_t dir = (msg[0] & DIRECTION_MASK) >> DIRECTION_OFFSET;

    if (spd >= static_cast<uint8_t>(TurnSpeed::_Count)
            || dir >= static_cast<uint8_t>(TurnDirection::_Count)) {
        // invalid enum
        return std::experimental::nullopt;
    }

    TurnSpeed speed = static_cast<TurnSpeed>(spd);
    TurnDirection direction = static_cast<TurnDirection>(dir);

    return {SimpleTurnMessage(speed, direction)};
}

/**
 * Constructs a SimpleTurnMessage
 *
 * @param speed     speed of the turn
 * @param direction direction of the turn
 */
SimpleTurnMessage::SimpleTurnMessage(const TurnSpeed& speed,
        const TurnDirection& direction) :
        Message(getMessageType()), speed(speed), direction(direction) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int SimpleTurnMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
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
void SimpleTurnMessage::print() const {
    printf("SimpleTurnCommand[speed=%s, direction=%s]", enumToString(speed),
            enumToString(direction));
}

/**
 * @return the turning speed
 */
const TurnSpeed& SimpleTurnMessage::getSpeed() {
    return speed;
}

/**
 * @return the turning direction
 */
const TurnDirection& SimpleTurnMessage::getDirection() {
    return direction;
}
/** @} */
