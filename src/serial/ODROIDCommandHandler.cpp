/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/ODROIDCommandHandler.h>
#include "scheduler/Scheduler.h"

/**
 * Constructs an ODROIDCommandHandler
 *
 * @param dispatcher the dispatcher to be used for processing the message
 */
ODROIDCommandHandler::ODROIDCommandHandler(MessageDispatcher& dispatcher) :
        currentBuffer(0), currentByte(0), expectedSize(0), dispatcher(dispatcher) {
}

/*
 * @see - UARTReceiveHandler::processByte(uint8_t)
 */
void ODROIDCommandHandler::processByte(uint8_t byte) {
    if (currentByte == 0) {
        // expecting header
        expectedSize = (byte & PAYLOAD_SIZE_MASK) + 1;
    }
    buffers[currentBuffer][currentByte] = byte;
    currentByte++;

    if (currentByte == expectedSize) {
        // received entire message
        uint8_t* msg = buffers[currentBuffer];
        int size = expectedSize;

        schedule_task_from_isr([this, msg, size]() {
            dispatcher.processMessage(msg, size);
        });

        currentBuffer = (currentBuffer + 1) % BUFFER_COUNT;
        currentByte = 0;
        expectedSize = 0;
    }

}
/** @} */
