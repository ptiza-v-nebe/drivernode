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
#include "serial/messages/Maybe.h"

class GameStartMessage : public Message {
public:
    static Maybe<GameStartMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0xE;
    }
public:
    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;
};

#endif /* SERIAL_MESSAGES_GAMESTARTMESSAGE_H_ */
/** @} */
