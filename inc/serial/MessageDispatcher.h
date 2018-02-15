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

#include "serial/messages/Maybe.h"
#include "serial/messages/Message.h"
#include "serial/MessageSender.h"
#include "serial/messages/message_info.h"
#include <functional>

constexpr int MESSAGE_TYPE_COUNT = MAX_TYPE + 1;

class MessageDispatcher {
private:
    std::function<void(uint8_t* msg, int size)> messageHandlers[MESSAGE_TYPE_COUNT];
    MessageSender& sender;
public:
    MessageDispatcher(MessageSender& sender);

    template<class Message>
    void registerMessageHandler(std::function<void(Message)> handler);

    void sendMessage(Message& message);

    void processMessage(uint8_t* msg, int size);
private:
    void sendAcknowledge();
    void sendInvalid();
    void sendUnknown();
};

template<class Message>
inline void MessageDispatcher::registerMessageHandler(
        std::function<void(Message)> handler) {
    // get message type as index of handler array
    int messageType = Message::getMessageType();
    if(messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // message type is valid --> register handler
        messageHandlers[messageType] = [this, handler](uint8_t* msg, int size) {
            Maybe<Message> result = Message::deserialize(msg, size);
            if(result.isValid()) {
                handler(result.get());
                sendAcknowledge();
            } else {
                sendInvalid();
            }
        };
    }
}

#endif /* SERIAL_MESSAGEDISPATCHER_H_ */
/** @} */
