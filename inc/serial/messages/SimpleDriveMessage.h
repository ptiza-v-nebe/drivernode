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
#include "util/optional.hpp"

class SimpleDriveMessage : public Message {
public:
    static std::experimental::optional<SimpleDriveMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x2;
    }
private:
    DriveSpeed speed;
    DriveDirection direction;
public:
    SimpleDriveMessage(const DriveSpeed& speed, const DriveDirection& direction);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const DriveSpeed& getSpeed();
    const DriveDirection& getDirection();
};

#endif /* SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_ */
/** @} */
