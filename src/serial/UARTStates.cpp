/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include "serial/UARTStates.h"
#include "serial/NucleoMessageDispatcher.h"
#include <serial/messages/all.h>
#include <serial/messages/version.h>

void UARTBaseState::heartbeatReceived(const HeartbeatMessage& msg) {
    if (msg.hasVersion()) {
        CHANGE_STATE(Init);
        context.publishComReset();
    }
}

void Init::heartbeatTick() {
    context.sendMessage(HeartbeatMessage(SERIAL_VERSION));
}

void Init::heartbeatReceived(const HeartbeatMessage& msg) {
    if (!msg.hasVersion()) {
        context.resetCount();
        CHANGE_STATE(WaitForConnection);
    }
}

void WaitForConnection::heartbeatTick() {
    context.sendMessage(HeartbeatMessage());
}

void WaitForConnection::heartbeatReceived(const HeartbeatMessage& msg) {
    if (msg.hasVersion()) {
        CHANGE_STATE(Init);
        context.publishComReset();
    } else {
        context.increaseCount();
        if (context.getCount() >= 5) {
            context.publishComEstablished();
            CHANGE_STATE(Connected);
        }
    }
}

void WaitForConnection::heartbeatTimeout() {
    if(context.getCount() == 0) { //TODO: double timeout to init...
        CHANGE_STATE(Init);
    }
}

void ReadyToSend::heartbeatTick() {
    if(!context.getMessageSent()) {
        context.sendMessage(HeartbeatMessage());
    }
    context.clearMessageSent();
}

void Connected::heartbeatTimeout() {
    context.publishComError();
    CHANGE_STATE(Init);
}

void Connected::messageReceived() {
    CHANGE_STATE(MessageReceived);
}

void MessageReceived::heartbeatTimeout() {
    CHANGE_STATE(Connected);
}
/** @} */
