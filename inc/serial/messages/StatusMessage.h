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
#include "serial/messages/Maybe.h"

#define STATUS_VALUES(m)\
    m(OK, 'O')\
    m(UNKNOWN, 'U')\
    m(INVALID, 'I')\
    m(ABORTED, 'A')\
    m(FINISHED, 'F')\
    m(STUCK, 'S')

SMARTENUM_SPECIAL_DEFINE_ENUM(Status, STATUS_VALUES)


constexpr int PAYLOAD_SIZE = 1;
class StatusMessage : public Message {
public:
    static Maybe<StatusMessage> deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x7;
    }
private:
    Status status;
public:
    StatusMessage(const Status& status);

    int serialize(uint8_t* buffer, int buffersize) override;
    void print() override;

    const Status& getStatus();
};

#endif /* SERIAL_MESSAGES_STATUSMESSAGE_H_ */
/** @} */
