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
#include "position/Position.h"

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
    Position position; ///< the target position
public:
    ControlledDriveMessage(const DriveSpeed& speed, const Position position);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const DriveSpeed& getSpeed();
    const Position& getPosition();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_ */
/** @} */
