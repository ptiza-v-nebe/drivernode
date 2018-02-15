/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_STOPMESSAGE_H_
#define SERIAL_MESSAGES_STOPMESSAGE_H_

#include "serial/messages/Message.h"
#include "serial/messages/Maybe.h"

class StopMessage : public Message {
public:
    static Maybe<StopMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x1;
    }
public:
    StopMessage() : Message(getMessageType()) {}
    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;
};

#endif /* SERIAL_MESSAGES_STOPMESSAGE_H_ */
/** @} */
