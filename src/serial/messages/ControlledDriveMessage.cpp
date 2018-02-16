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

static constexpr int PAYLOAD_SIZE = 5;

std::experimental::optional<ControlledDriveMessage> ControlledDriveMessage::deserialize(const uint8_t* msg,
        const int size) {
    if (size != PAYLOAD_SIZE) {
        // invalid message
        return std::experimental::nullopt;
    }

    if(msg[0] >= static_cast<uint8_t>(DriveSpeed::_Count)) {
        // invaid enum
        return std::experimental::nullopt;
    }
    DriveSpeed speed = static_cast<DriveSpeed>(msg[0]);
    uint16_t x, y;
    serialToSystem(msg + 1, x);
    serialToSystem(msg + 3, y);

    return {ControlledDriveMessage(speed, x, y)};
}

ControlledDriveMessage::ControlledDriveMessage(const DriveSpeed& speed,
        const uint16_t x, const uint16_t y) :
        Message(getMessageType()),
        speed(speed), x(x), y(y) {
}

int ControlledDriveMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = static_cast<uint8_t>(speed);
    if (systemToSerial(x, buffer + 1, 2) < 0) {
        return -1;
    }
    if (systemToSerial(y, buffer + 3, 2) < 0) {
        return -1;
    }

    return PAYLOAD_SIZE;
}

void ControlledDriveMessage::print() const {
    printf("ControlledDriveCommand[speed=%s, target=(%d, %d)]", enumToString(speed), x, y);
}

const DriveSpeed& ControlledDriveMessage::getSpeed() {
    return speed;
}

uint16_t ControlledDriveMessage::getX() {
    return x;
}

uint16_t ControlledDriveMessage::getY() {
    return y;
}
/** @} */
