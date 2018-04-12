/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_UARTWRAPPER_H_
#define SERIAL_UARTWRAPPER_H_

#include "serial/UARTReceiveHandler.h"
#include "serial/MessageSender.h"

#include <stm32l4xx.h>
#include <string>

/**
 * Wraps the USART2 which is connected to the ST-LINK interface into a C++ Object.
 */
class UARTWrapper: public MessageSender {
public:
    static UARTWrapper& getInstance();
private:
    UART_HandleTypeDef uart; ///< low level uart handle
    GPIO_InitTypeDef uart_gpio; ///< low level gpio handle
    UARTReceiveHandler *receiveHandler; ///< the receive handler to be notified about incoming bytes
private:
    UARTWrapper();
public:

    // prevent copy and move
    UARTWrapper(const UARTWrapper&) = delete;
    UARTWrapper(UARTWrapper&&) = delete;
    UARTWrapper& operator=(const UARTWrapper&) = delete;
    UARTWrapper& operator=(UARTWrapper&&) = delete;

    void send(const uint8_t* buffer, const int size) override;
    void handleByte(uint8_t byte);
    void setReceiveHandler(UARTReceiveHandler *receiveHandler);

private:
    void init();
};

#endif /* SERIAL_UARTWRAPPER_H_ */
/** @} */
