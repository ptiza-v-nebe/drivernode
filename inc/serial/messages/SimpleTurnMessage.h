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
#include "serial/messages/Maybe.h"

constexpr int PAYLOAD_SIZE = 1;

class SimpleTurnMessage : public Message {
public:
    static Maybe<SimpleTurnMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x4;
    }
private:
    TurnSpeed speed;
    TurnDirection direction;
public:
    SimpleTurnMessage(const TurnSpeed& speed, const TurnDirection& direction);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    const TurnSpeed& getSpeed();
    const TurnDirection& getDirection();
};

#endif /* SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_ */
/** @} */
