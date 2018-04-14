/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/NucleoMessageDispatcher.h>
#include "serial/messages/StatusMessage.h"

NucleoMessageDispatcher::NucleoMessageDispatcher(MessageSender& sender) : MessageDispatcher(sender) {
}

void NucleoMessageDispatcher::handleMessageProcessed() const {
    sendMessage(StatusMessage(Status::OK));
}

void NucleoMessageDispatcher::handleInvalidMessage() const {
    sendMessage(StatusMessage(Status::INVALID));
}

void NucleoMessageDispatcher::handleUnknownMessage() const {
    sendMessage(StatusMessage(Status::UNKNOWN));
}
/** @} */
