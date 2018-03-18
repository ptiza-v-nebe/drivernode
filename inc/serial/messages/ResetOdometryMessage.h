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
#include "util/optional.hpp"
#include "position/Position.h"
#include "position/Angle.h"

/**
 * Represents a ResetOdometryCommand.
 * Will reset the odometry position and heading.
 */
class ResetOdometryMessage : public Message {
public:
    static std::experimental::optional<ResetOdometryMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x8;
    }
private:
    Position position; ///< odometry position
    Angle heading; ///< odometry heading
public:
    ResetOdometryMessage(const Position position, const Angle heading);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const Position& getPosition();
    const Angle& getHeading();
};

#endif /* SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_ */
/** @} */
