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

#include <stm32l4xx.h>
#include <string>

class UARTWrapper {
private:
	UART_HandleTypeDef uart;
	GPIO_InitTypeDef uart_gpio;
public:
	UARTWrapper();
	virtual ~UARTWrapper();

	void send(std::string msg);
	std::string receive();

private:
	void init();
	void release();
};

#endif /* SERIAL_UARTWRAPPER_H_ */
/** @} */
