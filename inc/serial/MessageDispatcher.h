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

#include <functional>

constexpr int MESSAGE_TYPE_COUNT = 0xF + 1;

class MessageDispatcher {
private:
    std::function<void(uint8_t* msg, int size)> messageHandlers[MESSAGE_TYPE_COUNT];
public:
    MessageDispatcher();

    template<class Message>
    void registerMessageHandler(std::function<void(Message)> handler);

    void processMessage(uint8_t* msg, int size);
};

template<class Message>
inline void MessageDispatcher::registerMessageHandler(
        std::function<void(Message)> handler) {
    int messageType = Message::getMessageType();
    if(messageType > 0 && messageType < MESSAGE_TYPE_COUNT){
        messageHandlers[messageType] = [handler](uint8_t* msg, int size) {
            handler(Message::deserialize(msg, size));
        };
    }
}

#endif /* SERIAL_MESSAGEDISPATCHER_H_ */
/** @} */
