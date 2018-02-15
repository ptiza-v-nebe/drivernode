/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/StatusMessage.h>
#include <cstdio>

SMARTENUM_SPECIAL_DEFINE_NAMES(Status, STATUS_VALUES)

Maybe<StatusMessage> StatusMessage::deserialize(uint8_t* msg, int size) {
    if(size != PAYLOAD_SIZE){
        return Maybe<StatusMessage>::EMPTY;
    }

    Status status = getEnum<Status>(msg[0]);
    return {StatusMessage(status)};
}

StatusMessage::StatusMessage(const Status& status) : Message(getMessageType()), status(status) {
}

int StatusMessage::serialize(uint8_t* buffer, int buffersize) {
    if(buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = getSpecialValue(status);
    return PAYLOAD_SIZE;
}

void StatusMessage::print() {
    printf("StatusMessage[%s]", enumToString(status));
}

const Status& StatusMessage::getStatus() {
    return status;
}
/** @} */
