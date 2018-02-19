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

/**
 * The size of the payload expected for this message
 */
static constexpr int PAYLOAD_SIZE = 8;

/**
 * Deserialize a ResetOdometryMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a ResetOdometryMessage-object if the buffer contains a valid message
 */
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

/**
 * Constructs a ResetOdometryMessage.
 *
 * @param x       the new x position
 * @param y       the new y position
 * @param heading the new heading
 */
ResetOdometryMessage::ResetOdometryMessage(const uint16_t x, const uint16_t y,
        const float heading) :
        Message(getMessageType()), x(x), y(y), heading(heading) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
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

/*
 * @see - Message::print()
 */
void ResetOdometryMessage::print() const {
    printf("ResetOdometryCommand[position=(%d, %d), heading=%.2f°]", x, y,
            radiansToDegrees(heading));
}

/**
 * @return the new x position
 */
uint16_t ResetOdometryMessage::getX() {
    return x;
}

/**
 * @return the new y position
 */
uint16_t ResetOdometryMessage::getY() {
    return y;
}

/**
 * @return the new heading
 */
float ResetOdometryMessage::getHeading() {
    return heading;
}

/** @} */
