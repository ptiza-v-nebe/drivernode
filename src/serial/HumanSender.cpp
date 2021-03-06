/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/HumanSender.h>

#include "serial/messages/all.h"

/**
 * Constructs a HumanSender.
 */
HumanSender::HumanSender() {
    for (int i = 0; i < MESSAGE_TYPE_COUNT; i++) {
        deserializers[i] = [](const uint8_t*, const int) {
            printf("Unknown Message\r\n");
        };
    }
    initializeDeserializationTable();
}

/*
 * @see - MessageHandler::send(const uint8_t*, const int)
 */
void HumanSender::send(const uint8_t* message, const int size) {
    // get message type as offset for handler array
    int messageType = (message[0] & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
    if (messageType >= 0 && messageType < MESSAGE_TYPE_COUNT) {
        // pass payload to the handler which will deserialize and print it
        deserializers[messageType](message + 1, size - 1);
    }
}

/**
 * Initializes the deserialization table.
 * Uses template instantiation to transfer most of the work to the compiler :)
 */
void HumanSender::initializeDeserializationTable() {
    setDeseralizer<HeartbeatMessage>();
    setDeseralizer<StopMessage>();
    setDeseralizer<ControlledDriveMessage>();
    setDeseralizer<ControlledTurnMessage>();
    setDeseralizer<PositionMessage>();
    setDeseralizer<StatusMessage>();
    setDeseralizer<ResetOdometryMessage>();
    setDeseralizer<GameStartMessage>();
    setDeseralizer<SetSpeedMessage>();
}
/** @} */
