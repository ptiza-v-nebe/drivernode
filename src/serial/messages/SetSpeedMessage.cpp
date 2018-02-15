/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/SetSpeedMessage.h>
#include "serial/messages/util.h"
#include <cstdio>

static constexpr int PAYLOAD_SIZE_EQUAL = 2;
static constexpr int PAYLOAD_SIZE_BOTH = 4;

std::experimental::optional<SetSpeedMessage> SetSpeedMessage::deserialize(const uint8_t* msg, const int size) {
    switch (size) {
        case PAYLOAD_SIZE_EQUAL:
            uint16_t speed;
            serialToSystem(msg, speed);
            return {SetSpeedMessage(speed)};
        case PAYLOAD_SIZE_BOTH:
            uint16_t left, right;
            serialToSystem(msg, left);
            serialToSystem(msg + 2, right);
            return {SetSpeedMessage(left, right)};
        default:
            return std::experimental::nullopt;
    }
}

SetSpeedMessage::SetSpeedMessage(uint16_t speed) :
        SetSpeedMessage(speed, speed) {
}

SetSpeedMessage::SetSpeedMessage(uint16_t left, uint16_t right) :
        Message(getMessageType()), speedLeft(left), speedRight(right) {
}

int SetSpeedMessage::serialize(uint8_t* buffer, int buffersize) {
    if (speedLeft == speedRight) {
        return serializeEqual(buffer, buffersize);
    } else {
        return serializeBoth(buffer, buffersize);
    }
}

int SetSpeedMessage::serializeEqual(uint8_t* buffer, int buffersize) {
    if (buffersize < PAYLOAD_SIZE_EQUAL) {
        return -1;
    }

    if (systemToSerial(speedLeft, buffer, 2) < 0) {
        return -1;
    }
    return PAYLOAD_SIZE_EQUAL;
}

int SetSpeedMessage::serializeBoth(uint8_t* buffer, int buffersize) {
    if (buffersize < PAYLOAD_SIZE_BOTH) {
        return -1;
    }

    if (systemToSerial(speedLeft, buffer, 2) < 0) {
        return -1;
    }
    if (systemToSerial(speedRight, buffer + 2, 2) < 0) {
        return -1;
    }
    return PAYLOAD_SIZE_BOTH;
}

void SetSpeedMessage::print() {
    if (speedLeft == speedRight) {
        printf("SetSpeedComand[speed=%d]", speedLeft);
    } else {
        printf("SetSpeedComand[left=%d, right=%d]", speedLeft, speedRight);
    }
}

uint16_t SetSpeedMessage::getSpeedLeft() {
    return speedLeft;
}

uint16_t SetSpeedMessage::getSpeedRight() {
    return speedRight;
}
/** @} */
