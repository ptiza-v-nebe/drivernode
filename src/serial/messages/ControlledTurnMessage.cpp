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

static constexpr int PAYLOAD_SIZE = 5;

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

ControlledTurnMessage::ControlledTurnMessage(const TurnSpeed& speed,
        const float targetHeading) :
        Message(getMessageType()), speed(speed), targetHeading(targetHeading) {
}

int ControlledTurnMessage::serialize(uint8_t* buffer, int buffersize) const {
    if (buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = static_cast<uint8_t>(speed);
    systemToSerial(targetHeading, buffer + 1, 4);

    return PAYLOAD_SIZE;
}

void ControlledTurnMessage::print() const {
    printf("ControlledTurnCommand[speed=%s, target=%.2f]", enumToString(speed),
            radiansToDegrees(targetHeading));
}

const TurnSpeed& ControlledTurnMessage::getSpeed() {
    return speed;
}

float ControlledTurnMessage::getTargetHeading() {
    return targetHeading;
}
/** @} */
