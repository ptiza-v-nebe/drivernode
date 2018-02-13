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
#include "serial/MessageDispatcher.h"

constexpr int MAX_PAYLOAD = 0xF;
constexpr int MAX_MESSAGE_BYTES = MAX_PAYLOAD + 1;
constexpr int BUFFER_COUNT = 2;

class ODROIDCommandHandler: public UARTReceiveHandler {
private:
    uint8_t buffers[BUFFER_COUNT][MAX_MESSAGE_BYTES];
    int currentBuffer;
    int currentByte;
    int expectedSize;
    MessageDispatcher& dispatcher;
public:
    ODROIDCommandHandler(MessageDispatcher& dispatcher);

    void processByte(uint8_t byte) override;
};

#endif /* SERIAL_ODROIDCOMMANDHANDLER_H_ */
/** @} */
