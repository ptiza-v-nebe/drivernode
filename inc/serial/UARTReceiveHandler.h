/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_UARTRECEIVEHANDLER_H_
#define SERIAL_UARTRECEIVEHANDLER_H_

#include <cstdint>

class UARTReceiveHandler {
public:
    virtual ~UARTReceiveHandler() = default;

    /**
     * Process a byte that was received from the UART.
     * @attention This is called from an ISR! Keep it short!
     *
     * @param byte the byte that was received
     */
    virtual void processByte(uint8_t byte) = 0;
};

#endif /* SERIAL_UARTRECEIVEHANDLER_H_ */
/** @} */
