/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGEDISPATCHER_H_
#define SERIAL_MESSAGEDISPATCHER_H_

#include "util/optional.hpp"
#include "serial/messages/Message.h"
#include "serial/messages/MessageSender.h"
#include "serial/messages/message_info.h"
#include <functional>

class MessageDispatcher {
private:
    std::function<void(uint8_t* msg, int size)> messageHandlers[MESSAGE_TYPE_COUNT]; ///< lookup table for message handlers
    MessageSender& sender; ///< reference to the sender used to send messages
public:
    MessageDispatcher(MessageSender& sender);

    // prevent copy
    MessageDispatcher(const MessageDispatcher&) = delete;
    MessageDispatcher& operator= (const MessageDispatcher&) = delete;

    template<class Message>
    void registerMessageHandler(std::function<void(Message)> handler);

    void sendMessage(Message& message) const;

    void processMessage(uint8_t* msg, int size) const;
private:
    void sendAcknowledge() const;
    void sendInvalid() const;
    void sendUnknown() const;
};

/**
 * Registers a message handler for a specific message type.
 * Will wrap the message handler in a deserializer and store it in the lookup table.
 *
 * @param handler the function to be called when a new message of that type arrives
 */
template<class Message>
inline void MessageDispatcher::registerMessageHandler(
        std::function<void(Message)> handler) {
    // get message type as index of handler array
    int messageType = Message::getMessageType();
    if(messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // message type is valid --> register handler
        messageHandlers[messageType] = [this, handler](uint8_t* msg, int size) {
            auto result = Message::deserialize(msg, size);
            if(result.has_value()) {
                handler(*result);
                sendAcknowledge();
            } else {
                sendInvalid();
            }
        };
    }
}

#endif /* SERIAL_MESSAGEDISPATCHER_H_ */
/** @} */
