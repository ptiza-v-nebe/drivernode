/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_HUMANCOMMANDHANDLER_H_
#define SERIAL_HUMANCOMMANDHANDLER_H_

#include <serial/UARTReceiveHandler.h>
#include "serial/MessageDispatcher.h"

/**
 * The size of the message buffer
 */
constexpr int MSG_BUFFER_SIZE = 50;

/**
 * The number of message buffers.
 * Using multiple buffers allows processing of one message while already receiving the next one
 */
constexpr int MSG_BUFFER_COUNT = 2;

class HumanCommandHandler: public UARTReceiveHandler {
private:
    uint8_t buffers[MSG_BUFFER_COUNT][MSG_BUFFER_SIZE]; ///< the buffers.
    int currentBuffer = 0; ///< which buffer to write to
    int count = 0; ///< current buffer usage
    MessageDispatcher& dispatcher; ///< reference to the message dispatcher to which the message is passed
public:
    HumanCommandHandler(MessageDispatcher& dispatcher);

    void processByte(uint8_t byte) override;

private:
    bool isValidCharacter(uint8_t& byte);
    void sendLineBreak();
    void sendPrompt();

    void processMessage(uint8_t *msg, int size);
};

#endif /* SERIAL_HUMANCOMMANDHANDLER_H_ */
/** @} */
