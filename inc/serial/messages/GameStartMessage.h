/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_GAMESTARTMESSAGE_H_
#define SERIAL_MESSAGES_GAMESTARTMESSAGE_H_

#include "serial/messages/Message.h"
#include "util/optional.hpp"

class GameStartMessage : public Message {
public:
    static std::experimental::optional<GameStartMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0xE;
    }
public:
    GameStartMessage() : Message(getMessageType()) {}
    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;
};

#endif /* SERIAL_MESSAGES_GAMESTARTMESSAGE_H_ */
/** @} */
