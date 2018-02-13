/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/MessageDispatcher.h>

static auto DEFAULT_HANDLER = [](uint8_t*, int){};

static constexpr uint8_t MSG_TYPE_MASK = 0xF0;
static constexpr int MSG_TYPE_OFFSET = 4;

MessageDispatcher::MessageDispatcher() {
    for(int i = 0; i < MESSAGE_TYPE_COUNT; i++){
        messageHandlers[i] = DEFAULT_HANDLER;
    }
}

void MessageDispatcher::processMessage(uint8_t* msg, int size) {
    int messageType = (msg[0] & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
    if(messageType > 0 && messageType < MESSAGE_TYPE_COUNT) {
        // pass payload to serializer
        messageHandlers[messageType](msg + 1, size - 1);
    }
}
/** @} */
