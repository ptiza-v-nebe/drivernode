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

/**
 * The size of the payload expected for this message
 */
static constexpr int PAYLOAD_SIZE = 1;

/**
 * Deserialize a StatusMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a StatusMessage-object if the buffer contains a valid message
 */
std::experimental::optional<StatusMessage> StatusMessage::deserialize(const uint8_t* msg, const int size) {
    if(size != PAYLOAD_SIZE){
        return std::experimental::nullopt;
    }

    Status status = getEnum<Status>(msg[0]);
    if(status == Status::_Count){
        // invalid status code
        return std::experimental::nullopt;
    }
    return {StatusMessage(status)};
}

/**
 * Constructs a StatusMessage.
 *
 * @param status the status code to be represented
 */
StatusMessage::StatusMessage(const Status& status) : Message(getMessageType()), status(status) {
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int StatusMessage::serialize(uint8_t* buffer, int buffersize) const {
    if(buffersize < PAYLOAD_SIZE) {
        return -1;
    }

    buffer[0] = getSpecialValue(status);
    return PAYLOAD_SIZE;
}

/*
 * @see - Message::print()
 */
void StatusMessage::print() const {
    printf("StatusMessage[%s]", enumToString(status));
}

/**
 * @return the status code
 */
const Status& StatusMessage::getStatus() {
    return status;
}
/** @} */
