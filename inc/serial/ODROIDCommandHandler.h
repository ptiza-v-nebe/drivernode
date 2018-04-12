/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_ODROIDCOMMANDHANDLER_H_
#define SERIAL_ODROIDCOMMANDHANDLER_H_

#include <serial/UARTReceiveHandler.h>
#include "serial/messages/MessageDispatcher.h"
#include "serial/messages/message_info.h"

/**
 * Maximum size of a message
 */
constexpr int MAX_MESSAGE_BYTES = MAX_PAYLOAD + 1;

/**
 * Number of buffers.
 * Multiple buffers allow processing of one message while already receiving the next one
 */
constexpr int BUFFER_COUNT = 2;

class ODROIDCommandHandler: public UARTReceiveHandler {
private:
    uint8_t buffers[BUFFER_COUNT][MAX_MESSAGE_BYTES]; ///< receive buffer
    int currentBuffer; ///< which buffer to write to
    int currentByte; ///< which byte in the buffer to write to
    int expectedSize; ///< expected size of the message (set after receiving the header)
    MessageDispatcher& dispatcher; ///< reference to the message dispatcher to which the message is passed
public:
    ODROIDCommandHandler(MessageDispatcher& dispatcher);

    void processByte(uint8_t byte) override;
};

#endif /* SERIAL_ODROIDCOMMANDHANDLER_H_ */
/** @} */
