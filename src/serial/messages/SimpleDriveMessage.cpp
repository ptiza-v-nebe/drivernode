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

static constexpr uint8_t SPEED_MASK = 0x0F;
static constexpr uint8_t DIRECTION_MASK = 0xF0;
static constexpr int DIRECTION_OFFSET = 4;

Maybe<SimpleDriveMessage> SimpleDriveMessage::deserialize(uint8_t* msg,
        int size) {
    if (size != PAYLOAD_SIZE) {
        // something is wrong
        return Maybe<SimpleDriveMessage>::EMPTY;
    } else {
        DriveSpeed speed = static_cast<DriveSpeed>(msg[0] & SPEED_MASK);
        DriveDirection direction = static_cast<DriveDirection>((msg[0]
                & DIRECTION_MASK) >> DIRECTION_OFFSET);
        return { {speed, direction}};
    }
}

SimpleDriveMessage::SimpleDriveMessage(const DriveSpeed& speed,
        const DriveDirection& direction) :
        Message(getMessageType()), speed(speed), direction(direction) {
}

int SimpleDriveMessage::serialize(uint8_t* buffer, int buffersize) {
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

void SimpleDriveMessage::print() {
    printf("SimpleDriveComand[speed=%s, direction=%s]", enumToString(speed),
            enumToString(direction));
}

const DriveSpeed& SimpleDriveMessage::getSpeed() {
    return speed;
}

const DriveDirection& SimpleDriveMessage::getDirection() {
    return direction;
}
/** @} */
