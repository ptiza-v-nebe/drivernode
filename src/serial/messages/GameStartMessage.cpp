/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/GameStartMessage.h>
#include <cstdio>

/**
 * Deserialize a GameStartMessage.
 *
 * @param msg  buffers containing the message
 * @param size the size of the buffer
 *
 * @return a GameStartMessage-object if the buffer contains a valid message
 */
std::experimental::optional<GameStartMessage> GameStartMessage::deserialize(const uint8_t*, const int) {
    return {GameStartMessage()};
}

/*
 * @see - Message::serialize(uint8_t* int)
 */
int GameStartMessage::serialize(uint8_t*, int) const {
    return 0;
}

/*
 * @see - Message::print()
 */
void GameStartMessage::print() const {
    printf("GameStartCommand");
}
/** @} */
