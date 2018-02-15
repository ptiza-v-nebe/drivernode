/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_HUMANSENDER_H_
#define SERIAL_HUMANSENDER_H_

#include <serial/MessageSender.h>

#include "util/optional.hpp"
#include "serial/messages/Message.h"
#include "serial/messages/message_info.h"
#include <functional>

class HumanSender: public MessageSender {
private:
    std::function<void(const uint8_t* msg, const int size)> deserializers[MESSAGE_TYPE_COUNT];
public:
    HumanSender();
    void send(const uint8_t* message, const int size) override;

private:
    void initializeDeserializationTable();

    template<class Message>
    void setDeseralizer();
};

void printBytes(const uint8_t* bytes, const int length);

template<class Message>
inline void HumanSender::setDeseralizer() {
    // get message type as index of handler array
    int messageType = Message::getMessageType();
    if (messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // message type is valid --> register handler
        deserializers[messageType] = [](const uint8_t* msg, const int size) {
            auto result = Message::deserialize(msg, size);
            if(result.has_value()) {
                result->print();
                printf(" - ");
                printBytes(msg - 1, size + 1); // Buffer underrun is ok here, because I know that header is at the front
                printf("\r\n");
            } else {
                printf("Invalid Serial Message\r\n");
            }
        };
    }
}

#endif /* SERIAL_HUMANSENDER_H_ */
/** @} */
