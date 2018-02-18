/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_
#define SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_

#include "serial/messages/drive_types.h"

#include "serial/messages/Message.h"
#include "util/optional.hpp"

/**
 * Represents a ControlledTurnCommand.
 * Uses the "regler" to turn to a specific heading.
 */
class ControlledTurnMessage : public Message {
public:
    static std::experimental::optional<ControlledTurnMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x5;
    }
private:
    TurnSpeed speed; ///< the speed of the turn
    float targetHeading; ///< the target heading to be reached
public:
    ControlledTurnMessage(const TurnSpeed& speed, const float targetHeading);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const TurnSpeed& getSpeed();
    float getTargetHeading();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_ */
/** @} */
