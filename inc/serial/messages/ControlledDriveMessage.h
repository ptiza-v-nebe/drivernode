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

/**
 * Represents a ControlledDriveCommand.
 * Uses the "regler" to drive to a specific position
 */
class ControlledDriveMessage : public Message {
public:
    static std::experimental::optional<ControlledDriveMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x3;
    }
private:
    DriveSpeed speed; ///< the speed of the drive
    uint16_t x; ///< the target x position
    uint16_t y; ///< the target y position
public:
    ControlledDriveMessage(const DriveSpeed& speed, const uint16_t x, const uint16_t y);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const DriveSpeed& getSpeed();
    uint16_t getX();
    uint16_t getY();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_ */
/** @} */
