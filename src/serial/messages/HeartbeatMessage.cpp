/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/HeartbeatMessage.h>

#include <cstdio>

Maybe<HeartbeatMessage>HeartbeatMessage::deserialize(uint8_t*, int) {
    return {HeartbeatMessage()};
}

int HeartbeatMessage::serialize(uint8_t*, int) {
    return 0;
}

void HeartbeatMessage::print() {
    printf("Heartbeat");
}
/** @} */
