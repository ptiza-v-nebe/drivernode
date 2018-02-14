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

Maybe<StopMessage> StopMessage::deserialize(uint8_t*, int) {
    return {StopMessage()};
}

int StopMessage::serialize(uint8_t*, int) {
    return 0;
}

void StopMessage::print() {
    printf("StopCommand");
}
/** @} */
