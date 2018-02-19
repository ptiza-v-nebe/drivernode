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

/**
 * Deserialize a HeartbeatMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a HeartbeatMessage-object if the buffer contains a valid message
 */
std::experimental::optional<HeartbeatMessage>HeartbeatMessage::deserialize(const uint8_t*, const int) {
    return {HeartbeatMessage()};
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int HeartbeatMessage::serialize(uint8_t*, int) const {
    return 0;
}

/*
 * @see - Message::print()
 */
void HeartbeatMessage::print() const {
    printf("Heartbeat");
}
/** @} */
