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

std::experimental::optional<GameStartMessage> GameStartMessage::deserialize(const uint8_t*, const int) {
    return {GameStartMessage()};
}

int GameStartMessage::serialize(uint8_t*, int) const {
    return 0;
}

void GameStartMessage::print() const {
    printf("GameStartCommand");
}
/** @} */
