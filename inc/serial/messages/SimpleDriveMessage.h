/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_
#define SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_

#include "serial/messages/drive_types.h"

#include "serial/messages/Message.h"
#include "serial/messages/Maybe.h"

constexpr int PAYLOAD_SIZE = 1;

class SimpleDriveMessage : public Message {
public:
    static Maybe<SimpleDriveMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x2;
    }
private:
    DriveSpeed speed;
    DriveDirection direction;
public:
    SimpleDriveMessage(const DriveSpeed& speed, const DriveDirection& direction);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    const DriveSpeed& getSpeed();
    const DriveDirection& getDirection();
};

#endif /* SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_ */
/** @} */
