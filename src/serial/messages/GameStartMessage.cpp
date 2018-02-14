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

Maybe<GameStartMessage> GameStartMessage::deserialize(uint8_t*, int) {
    return {GameStartMessage()};
}

int GameStartMessage::serialize(uint8_t*, int) {
    return 0;
}

void GameStartMessage::print() {
    printf("GameStartCommand");
}
/** @} */
