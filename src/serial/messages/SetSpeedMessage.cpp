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

/**
 * Expected payload size if only one speed is sent for both motors
 */
static constexpr int PAYLOAD_SIZE_EQUAL = 2;

/**
 * Expected payload size if both motors get different speeds
 */
static constexpr int PAYLOAD_SIZE_BOTH = 4;

/**
 * Deserialize a SetSpeedMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a SetSpeedMessage-object if the buffer contains a valid message
 */
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

/**
 * Constructs a SetSpeedMessage
 *
 * @param speed the speed to be set for both motors.
 */
SetSpeedMessage::SetSpeedMessage(uint16_t speed) :
        SetSpeedMessage(speed, speed) {
}

/**
 * Constructs a SetSpeedMessage.
 *
 * @param left  speed for the left motor
 * @param right speed for the right motor
 */
SetSpeedMessage::SetSpeedMessage(uint16_t left, uint16_t right) :
        Message(getMessageType()), speedLeft(left), speedRight(right) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int SetSpeedMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (speedLeft == speedRight) {
        return serializeEqual(buffer, buffersize);
    } else {
        return serializeBoth(buffer, buffersize);
    }
}

/**
 * Serialize message for equal speeds.
 *
 * @param buffer     the buffer to serialize into
 * @param buffersize size of the buffer
 *
 * @return number of bytes used
 * @retval -1 not enough space
 */
int SetSpeedMessage::serializeEqual(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE_EQUAL) {
        return -1;
    }

    if (systemToSerial(speedLeft, buffer, 2) < 0) {
        return -1;
    }
    return PAYLOAD_SIZE_EQUAL;
}

/**
 * Serialize message for different speeds.
 *
 * @param buffer     the buffer to serialize into
 * @param buffersize the size of the buffer
 *
 * @return number of bytes used
 * @retval -1 not enough space
 */
int SetSpeedMessage::serializeBoth(uint8_t* buffer, int buffersize) const {
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

/*
 * @see - Message::print()
 */
void SetSpeedMessage::print() const {
    if (speedLeft == speedRight) {
        printf("SetSpeedComand[speed=%d]", speedLeft);
    } else {
        printf("SetSpeedComand[left=%d, right=%d]", speedLeft, speedRight);
    }
}

/**
 * @return the speed for the left motor
 */
uint16_t SetSpeedMessage::getSpeedLeft() {
    return speedLeft;
}

/**
 * @return the speed for the right motor
 */
uint16_t SetSpeedMessage::getSpeedRight() {
    return speedRight;
}
/** @} */
