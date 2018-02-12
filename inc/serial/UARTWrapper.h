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

#include <stm32l4xx.h>
#include <string>

class UARTWrapper {
public:
    static UARTWrapper& getInstance();
private:
	UART_HandleTypeDef uart;
	GPIO_InitTypeDef uart_gpio;
	UARTReceiveHandler *receiveHandler;
private:
	UARTWrapper();

public:
	void send(std::string msg);
	void send(const uint8_t* buffer, const int size);
	void handleByte(uint8_t byte);
	void setReceiveHandler(UARTReceiveHandler *receiveHandler);

private:
	void init();
};

#endif /* SERIAL_UARTWRAPPER_H_ */
/** @} */
