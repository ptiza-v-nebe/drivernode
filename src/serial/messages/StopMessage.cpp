/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/StopMessage.h>

#include <cstdio>

/**
 * Deserialize a StopMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a StopMessage-object if the buffer contains a valid message
 */
std::experimental::optional<StopMessage> StopMessage::deserialize(const uint8_t*, const int) {
    return {StopMessage()};
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int StopMessage::serialize(uint8_t*, int) const {
    return 0;
}

/*
 * @see - Message::print()
 */
void StopMessage::print() const {
    printf("StopCommand");
}
/** @} */
