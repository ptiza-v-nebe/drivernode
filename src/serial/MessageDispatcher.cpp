/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/MessageDispatcher.h>
#include "serial/messages/StatusMessage.h"
#include "serial/Serializer.h"

MessageDispatcher::MessageDispatcher(MessageSender& sender) :
        sender(sender) {
    for (int i = 0; i < MESSAGE_TYPE_COUNT; i++) {
        messageHandlers[i] = [this](uint8_t*, int) {
            sendUnknown();
        };
    }
}

void MessageDispatcher::processMessage(uint8_t* msg, int size) const {
    // get message type as offset for handler array
    int messageType = (msg[0] & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
    if (messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // pass payload to the handler which will deserialize it and then call the actual handler
        messageHandlers[messageType](msg + 1, size - 1);
    }
}

void MessageDispatcher::sendMessage(Message& message) const {
    static uint8_t buffer[MAX_PAYLOAD + 1];

    int result = Serializer::serialize(message, buffer, MAX_PAYLOAD + 1);
    if(result > 0) {
        sender.send(buffer, result);
    }
}

void MessageDispatcher::sendAcknowledge() const {
    StatusMessage m(Status::OK);
    sendMessage(m);
}

void MessageDispatcher::sendInvalid() const {
    StatusMessage m(Status::INVALID);
    sendMessage(m);
}

void MessageDispatcher::sendUnknown() const {
    StatusMessage m(Status::UNKNOWN);
    sendMessage(m);
}
/** @} */
