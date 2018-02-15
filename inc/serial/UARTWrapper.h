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

class UARTWrapper : public MessageSender {
public:
    static UARTWrapper& getInstance();
private:
	UART_HandleTypeDef uart; ///< low level uart handle
	GPIO_InitTypeDef uart_gpio; ///< low level gpio handle
	UARTReceiveHandler *receiveHandler; ///< the receive handler to be notified about incoming bytes
private:
	UARTWrapper();

public:
	void send(const uint8_t* buffer, const int size) override;
	void handleByte(uint8_t byte);
	void setReceiveHandler(UARTReceiveHandler *receiveHandler);

private:
	void init();
};

#endif /* SERIAL_UARTWRAPPER_H_ */
/** @} */
