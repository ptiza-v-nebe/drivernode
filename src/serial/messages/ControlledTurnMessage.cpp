/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/ControlledTurnMessage.h>
#include "serial/messages/util.h"
#include "util/conversions.h"
#include <cstdio>

/**
 * The size of the payload expected for this message
 */
static constexpr int PAYLOAD_SIZE = 5;

/**
 * Deserialize a ControlledTurnMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a ControlledTurnMessage-object if the buffer contains a valid message
 */
std::experimental::optional<ControlledTurnMessage> ControlledTurnMessage::deserialize(
        const uint8_t* msg, const int size) {
    if (size != PAYLOAD_SIZE) {
        return std::experimental::nullopt;
    }

    if (msg[0] >= static_cast<uint8_t>(TurnSpeed::_Count)) {
        // invaid enum
        return std::experimental::nullopt;
    }

    TurnSpeed speed = static_cast<TurnSpeed>(msg[0]);
    float targetHeading;
    serialToSystem(msg + 1, targetHeading);

    return {ControlledTurnMessage(speed, targetHeading)};
}

/**
 * Constructs a ControlledTurnMessage.
 *
 * @param speed         the speed of the turn
 * @param targetHeading the target heading
 */
ControlledTurnMessage::ControlledTurnMessage(const TurnSpeed& speed,
        const float targetHeading) :
        Message(getMessageType()), speed(speed), targetHeading(targetHeading) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int ControlledTurnMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = static_cast<uint8_t>(speed);
    systemToSerial(targetHeading, buffer + 1, 4);

    return PAYLOAD_SIZE;
}

/*
 * @see - Message::print()
 */
void ControlledTurnMessage::print() const {
    printf("ControlledTurnCommand[speed=%s, target=%.2f°]", enumToString(speed),
            radiansToDegrees(targetHeading));
}

/**
 * @return the turn speed
 */
const TurnSpeed& ControlledTurnMessage::getSpeed() {
    return speed;
}

/**
 * @return the target heading after the turn
 */
float ControlledTurnMessage::getTargetHeading() {
    return targetHeading;
}
/** @} */
