/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/PositionMessage.h>
#include "serial/messages/util.h"
#include "util/conversions.h"
#include <cstdio>

Maybe<PositionMessage> PositionMessage::deserialize(uint8_t* msg, int size) {
    if (size != PAYLOAD_SIZE) {
        return Maybe<PositionMessage>::EMPTY;
    }

    uint16_t x, y;
    float heading;

    serialToSystem(msg, x);
    serialToSystem(msg + 2, y);
    serialToSystem(msg + 4, heading);

    return {PositionMessage(x,y,heading)};
}

PositionMessage::PositionMessage(const uint16_t x, const uint16_t y,
        const float heading) :
        x(x), y(y), heading(heading) {
}

int PositionMessage::serialize(uint8_t* buffer, int buffersize) {
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

void PositionMessage::print() {
    printf("PositionMessage[position=(%d, %d), heading=%.2f]", x, y, radiansToDegrees(heading)); // @suppress("Float formatting support")
}

uint16_t PositionMessage::getX() {
    return x;
}

uint16_t PositionMessage::getY() {
    return y;
}

float PositionMessage::getHeading() {
    return heading;
}
/** @} */
