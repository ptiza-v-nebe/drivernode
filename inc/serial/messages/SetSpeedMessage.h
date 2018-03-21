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

/**
 * Represents a SetSpeed command.
 * Used in conjunction with SimpleTurn and SimpleDrive - will set the exact speed for the motors.
 */
class SetSpeedMessage: public Message {
public:
    static std::experimental::optional<SetSpeedMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0xF;
    }
private:
    int16_t speedLeft; ///< exact speed for left motor
    int16_t speedRight; ///< exact speed for right motor
public:
    SetSpeedMessage(int16_t speed);
    SetSpeedMessage(int16_t left, int16_t right);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    int16_t getSpeedLeft();
    int16_t getSpeedRight();

private:
    int serializeEqual(uint8_t* buffer, int buffersize) const;
    int serializeBoth(uint8_t* buffer, int buffersize) const;
};

#endif /* SERIAL_MESSAGES_SETSPEEDMESSAGE_H_ */
/** @} */
