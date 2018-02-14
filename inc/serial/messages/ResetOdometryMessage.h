/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_
#define SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_

#include "serial/messages/Message.h"
#include "serial/messages/Maybe.h"

constexpr int PAYLOAD_SIZE = 8;

class ResetOdometryMessage : public Message {
public:
    static Maybe<ResetOdometryMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x8;
    }
private:
    uint16_t x;
    uint16_t y;
    float heading;
public:
    ResetOdometryMessage(const uint16_t x, const uint16_t y, const float heading);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    uint16_t getX();
    uint16_t getY();
    float getHeading();
};

#endif /* SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_ */
/** @} */
