/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_HEARTBEATMESSAGE_H_
#define SERIAL_MESSAGES_HEARTBEATMESSAGE_H_

#include "serial/messages/Message.h"
#include "serial/messages/Maybe.h"

class HeartbeatMessage : public Message {
public:
    static Maybe<HeartbeatMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x0;
    }
public:
    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;
};

#endif /* SERIAL_MESSAGES_HEARTBEATMESSAGE_H_ */
/** @} */
