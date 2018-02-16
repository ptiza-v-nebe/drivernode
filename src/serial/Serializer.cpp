/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/Serializer.h>
#include "serial/messages/message_info.h"

int Serializer::serialize(const Message& message, uint8_t* buffer,
        const int buffersize) {
    if (buffersize < 1) {
        // cannot serialize without buffer
        return -1;
    }

    // serialize message into payload part of buffer
    int result = message.serialize(buffer + 1, buffersize - 1);
    if (result < 0) {
        // something went wrong (probably didn't fit)
        return -1;
    } else {
        // calculate header
        uint8_t header = 0;
        header |= (message.getType() << MSG_TYPE_OFFSET) & MSG_TYPE_MASK;
        header |= result & PAYLOAD_SIZE_MASK;

        buffer[0] = header;

        return result + 1;
    }
}
/** @} */
