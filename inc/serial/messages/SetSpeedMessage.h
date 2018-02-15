/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SETSPEEDMESSAGE_H_
#define SERIAL_MESSAGES_SETSPEEDMESSAGE_H_

#include "serial/messages/Message.h"
#include "util/optional.hpp"

class SetSpeedMessage: public Message {
public:
    static std::experimental::optional<SetSpeedMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0xF;
    }
private:
    uint16_t speedLeft;
    uint16_t speedRight;
public:
    SetSpeedMessage(uint16_t speed);
    SetSpeedMessage(uint16_t left, uint16_t right);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    uint16_t getSpeedLeft();
    uint16_t getSpeedRight();

private:
    int serializeEqual(uint8_t* buffer, int buffersize);
    int serializeBoth(uint8_t* buffer, int buffersize);
};

#endif /* SERIAL_MESSAGES_SETSPEEDMESSAGE_H_ */
/** @} */
