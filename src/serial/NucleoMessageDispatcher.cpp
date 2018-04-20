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
#include <serial/UARTStates.h>
#include <serial/messages/HeartbeatMessage.h>
#include <serial/ComStatusHandler.h>
#include <scheduler/Scheduler.h>
#include <config.h>

static constexpr int HEARTBEAT_TICK_MS = 200;
static constexpr int HEARTBEAT_TIMEOUT_MS = 500;

NucleoMessageDispatcher::NucleoMessageDispatcher(MessageSender& sender) :
        MessageDispatcher(sender), currentState(new Init(*this)), count(0), messageSent(
                false) {

    registerMessageHandler<HeartbeatMessage>(
            [this](const HeartbeatMessage& msg) {
                currentState->heartbeatReceived(msg);
            });

#ifndef HUMAN_MODE
    schedule_repeating_task([this]() {
        currentState->heartbeatTick();
    }, HEARTBEAT_TICK_MS);

    schedule_repeating_task([this]() {
        currentState->heartbeatTimeout();
    }, HEARTBEAT_TIMEOUT_MS);
#endif
}

NucleoMessageDispatcher::~NucleoMessageDispatcher() {
    delete currentState;
}

void NucleoMessageDispatcher::handleMessageProcessed(uint8_t type) const {
    if (type != HeartbeatMessage::getMessageType()) {
        sendMessage(StatusMessage(Status::OK));
    }
    currentState->messageReceived();
}

void NucleoMessageDispatcher::handleInvalidMessage() const {
    sendMessage(StatusMessage(Status::INVALID));
}

void NucleoMessageDispatcher::handleUnknownMessage() const {
    sendMessage(StatusMessage(Status::UNKNOWN));
}

void NucleoMessageDispatcher::resetCount() {
    count = 0;
}

void NucleoMessageDispatcher::increaseCount() {
    count++;
}

int NucleoMessageDispatcher::getCount() {
    return count;
}

void NucleoMessageDispatcher::setMessageSent() const {
    messageSent = true;
}

void NucleoMessageDispatcher::clearMessageSent() {
    messageSent = false;
}

bool NucleoMessageDispatcher::getMessageSent() {
    return messageSent;
}

void NucleoMessageDispatcher::publishComError() {
    if (statusHandler) {
        statusHandler->handleComFailed();
    }
}

void NucleoMessageDispatcher::publishComEstablished() {
    if (statusHandler) {
        statusHandler->handleComEstablished();
    }
}

void NucleoMessageDispatcher::publishComReset() {
    if (statusHandler) {
        statusHandler->handleComReset();
    }
}

void NucleoMessageDispatcher::sendMessage(const Message& msg) const {
    MessageDispatcher::sendMessage(msg);
    setMessageSent();
}
/** @} */
