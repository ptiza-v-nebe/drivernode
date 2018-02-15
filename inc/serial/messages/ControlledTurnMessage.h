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

class ControlledTurnMessage : public Message {
public:
    static std::experimental::optional<ControlledTurnMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x5;
    }
private:
    TurnSpeed speed;
    float targetHeading;
public:
    ControlledTurnMessage(const TurnSpeed& speed, const float targetHeading);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    const TurnSpeed& getSpeed();
    const float getTargetHeading();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_ */
/** @} */
