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
    uint16_t x; ///< current x position
    uint16_t y; ///< current < position
    float heading; ///< current heading
public:
    PositionMessage(const uint16_t x, const uint16_t y, const float heading);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    uint16_t getX();
    uint16_t getY();
    float getHeading();
};

#endif /* SERIAL_MESSAGES_POSITIONMESSAGE_H_ */
/** @} */
