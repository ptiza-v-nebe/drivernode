/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_
#define SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_

#include "serial/messages/drive_types.h"

#include "serial/messages/Message.h"
#include "util/optional.hpp"

class ControlledDriveMessage : public Message {
public:
    static std::experimental::optional<ControlledDriveMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x3;
    }
private:
    DriveSpeed speed;
    uint16_t x;
    uint16_t y;
public:
    ControlledDriveMessage(const DriveSpeed& speed, const uint16_t x, const uint16_t y);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    const DriveSpeed& getSpeed();
    uint16_t getX();
    uint16_t getY();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_ */
/** @} */
