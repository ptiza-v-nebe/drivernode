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
#include "serial/messages/Maybe.h"

constexpr int PAYLOAD_SIZE = 5;

class ControlledTurnMessage : public Message {
public:
    static Maybe<ControlledTurnMessage> deserialize(uint8_t* msg, int size);
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
