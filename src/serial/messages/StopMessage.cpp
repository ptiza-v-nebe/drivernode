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

std::experimental::optional<StopMessage> StopMessage::deserialize(const uint8_t*, const int) {
    return {StopMessage()};
}

int StopMessage::serialize(uint8_t*, int) {
    return 0;
}

void StopMessage::print() {
    printf("StopCommand");
}
/** @} */
