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

std::experimental::optional<HeartbeatMessage>HeartbeatMessage::deserialize(const uint8_t*, const int) {
    return {HeartbeatMessage()};
}

int HeartbeatMessage::serialize(uint8_t*, int) const {
    return 0;
}

void HeartbeatMessage::print() const {
    printf("Heartbeat");
}
/** @} */
