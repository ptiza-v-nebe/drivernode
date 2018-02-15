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

Maybe<SimpleTurnMessage> SimpleTurnMessage::deserialize(uint8_t* msg,
        int size) {
    if (size != PAYLOAD_SIZE) {
        // invalid
        return Maybe<SimpleTurnMessage>::EMPTY;
    }

    TurnSpeed speed = static_cast<TurnSpeed>(msg[0] & SPEED_MASK);
    TurnDirection direction = static_cast<TurnDirection>((msg[0]
            & DIRECTION_MASK) >> DIRECTION_OFFSET);

    return {SimpleTurnMessage(speed, direction)};
}

SimpleTurnMessage::SimpleTurnMessage(const TurnSpeed& speed,
        const TurnDirection& direction) :
        Message(getMessageType()), speed(speed), direction(direction) {
}

int SimpleTurnMessage::serialize(uint8_t* buffer, int buffersize) {
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

void SimpleTurnMessage::print() {
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
