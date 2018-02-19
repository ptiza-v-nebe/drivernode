/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_
#define SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_

#include "serial/messages/drive_types.h"

#include "serial/messages/Message.h"
#include "util/optional.hpp"

/**
 * Represents a SimpleTurn command.
 * Will send the robot into an endless turn until another driving related message is received.
 */
class SimpleTurnMessage : public Message {
public:
    static std::experimental::optional<SimpleTurnMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x4;
    }
private:
    TurnSpeed speed; ///< the turn speed
    TurnDirection direction; ///< the direction of the turn
public:
    SimpleTurnMessage(const TurnSpeed& speed, const TurnDirection& direction);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const TurnSpeed& getSpeed();
    const TurnDirection& getDirection();
};

#endif /* SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_ */
/** @} */
