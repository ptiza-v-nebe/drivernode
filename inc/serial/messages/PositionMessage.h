/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_POSITIONMESSAGE_H_
#define SERIAL_MESSAGES_POSITIONMESSAGE_H_

#include "serial/messages/Message.h"
#include "util/optional.hpp"

#include "position/Position.h"
#include "position/Angle.h"

/**
 * Represents a PositionUpdateMessage.
 * This is sent by the robot containing the current odometry position and heading
 */
class PositionMessage : public Message {
public:
    static std::experimental::optional<PositionMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x6;
    }
private:
    Position position; ///< current position
    Angle heading; ///< current heading
public:
    PositionMessage(const Position position, const Angle heading);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const Position& getPosition();
    const Angle& getHeading();
};

#endif /* SERIAL_MESSAGES_POSITIONMESSAGE_H_ */
/** @} */
