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
#include "util/optional.hpp"

class HeartbeatMessage : public Message {
public:
    static std::experimental::optional<HeartbeatMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x0;
    }
public:
    HeartbeatMessage() : Message(getMessageType()) {}
    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;
};

#endif /* SERIAL_MESSAGES_HEARTBEATMESSAGE_H_ */
/** @} */
