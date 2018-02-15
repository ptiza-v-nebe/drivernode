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
#include "util/optional.hpp"

class StopMessage : public Message {
public:
    static std::experimental::optional<StopMessage> deserialize(const uint8_t* msg, const int size);
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
