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

MessageDispatcher::MessageDispatcher(MessageSender& sender) :
        sender(sender) {
    for (int i = 0; i < MESSAGE_TYPE_COUNT; i++) {
        messageHandlers[i] = [this](uint8_t*, int) {
            sendUnknown();
        };
    }
}

void MessageDispatcher::processMessage(uint8_t* msg, int size) {
    // get message type as offset for handler array
    int messageType = (msg[0] & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
    if (messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // pass payload to the handler which will deserialize it and then call the actual handler
        messageHandlers[messageType](msg + 1, size - 1);
    }
}

void MessageDispatcher::sendMessage(Message& message) {
    static uint8_t buffer[MAX_PAYLOAD + 1];

    // serialize message into payload part of buffer
    int result = message.serialize(buffer + 1, MAX_PAYLOAD);
    if(result < 0 || result > MAX_PAYLOAD) {
        // something went wrong
    } else {
        // calculate header
        uint8_t header = 0;
        header |= (message.getType() << MSG_TYPE_OFFSET) & MSG_TYPE_MASK;
        header |= result & PAYLOAD_SIZE_MASK;

        buffer[0] = header;

        sender.send(buffer, result + 1);
    }
}

void MessageDispatcher::sendAcknowledge() {
    StatusMessage m(Status::OK);
    sendMessage(m);
}

void MessageDispatcher::sendInvalid() {
    StatusMessage m(Status::INVALID);
    sendMessage(m);
}

void MessageDispatcher::sendUnknown() {
    StatusMessage m(Status::UNKNOWN);
    sendMessage(m);
}
/** @} */
