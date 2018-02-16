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

static constexpr uint8_t SPEED_MASK = 0x0F;
static constexpr uint8_t DIRECTION_MASK = 0xF0;
static constexpr int DIRECTION_OFFSET = 4;

static constexpr int PAYLOAD_SIZE = 1;

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

SimpleTurnMessage::SimpleTurnMessage(const TurnSpeed& speed,
        const TurnDirection& direction) :
        Message(getMessageType()), speed(speed), direction(direction) {
}

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

void SimpleTurnMessage::print() const {
    printf("SimpleTurnCommand[speed=%s, direction=%s]", enumToString(speed),
            enumToString(direction));
}

const TurnSpeed& SimpleTurnMessage::getSpeed() {
    return speed;
}

const TurnDirection& SimpleTurnMessage::getDirection() {
    return direction;
}
/** @} */
