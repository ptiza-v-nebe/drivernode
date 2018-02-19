/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_STATUSMESSAGE_H_
#define SERIAL_MESSAGES_STATUSMESSAGE_H_

#include "util/enum_to_string.h"

#include "serial/messages/Message.h"
#include "util/optional.hpp"

#define STATUS_VALUES(m)\
    m(OK, 'O')\
    m(UNKNOWN, 'U')\
    m(INVALID, 'I')\
    m(ABORTED, 'A')\
    m(FINISHED, 'F')\
    m(STUCK, 'S')

SMARTENUM_SPECIAL_DEFINE_ENUM(Status, STATUS_VALUES)

/**
 * Represents a status code.
 */
class StatusMessage : public Message {
public:
    static std::experimental::optional<StatusMessage> deserialize(const uint8_t* msg, const int size);
    static int getMessageType() {
        return 0x7;
    }
private:
    Status status; ///< the status code
public:
    StatusMessage(const Status& status);

    int serialize(uint8_t* buffer, int buffersize) const override;
    void print() const override;

    const Status& getStatus();
};

#endif /* SERIAL_MESSAGES_STATUSMESSAGE_H_ */
/** @} */
