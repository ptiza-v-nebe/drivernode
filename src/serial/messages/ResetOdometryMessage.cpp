/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/ResetOdometryMessage.h>
#include "serial/messages/util.h"
#include "util/conversions.h"
#include <cstdio>

static constexpr int PAYLOAD_SIZE = 8;

std::experimental::optional<ResetOdometryMessage> ResetOdometryMessage::deserialize(const uint8_t* msg,
        const int size) {
    if (size != PAYLOAD_SIZE) {
        return std::experimental::nullopt;
    }

    uint16_t x, y;
    float heading;

    serialToSystem(msg, x);
    serialToSystem(msg + 2, y);
    serialToSystem(msg + 4, heading);

    return {ResetOdometryMessage(x,y,heading)};
}

ResetOdometryMessage::ResetOdometryMessage(const uint16_t x, const uint16_t y,
        const float heading) :
        Message(getMessageType()), x(x), y(y), heading(heading) {
}

int ResetOdometryMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    if (systemToSerial(x, buffer, 2) < 0) {
        return -1;
    }
    if (systemToSerial(y, buffer + 2, 2) < 0) {
        return -1;
    }
    if (systemToSerial(heading, buffer + 4, 4) < 0) {
        return -1;
    }
    return PAYLOAD_SIZE;
}

void ResetOdometryMessage::print() const {
    printf("ResetOdometryCommand[position=(%d, %d), heading=%.2f°]", x, y,
            radiansToDegrees(heading));
}

uint16_t ResetOdometryMessage::getX() {
    return x;
}

uint16_t ResetOdometryMessage::getY() {
    return y;
}

float ResetOdometryMessage::getHeading() {
    return heading;
}

/** @} */
