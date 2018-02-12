/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/UARTWrapper.h>

#include "hal/util.h"


static constexpr uint32_t DEFAULT_TIMEOUT_MS = 500;

UARTWrapper::UARTWrapper() {
	init();

}

UARTWrapper::~UARTWrapper() {
	release();
}

void UARTWrapper::init() {
	uart.Instance = USART2;

	uart.Init.BaudRate = 115200;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_ENABLED;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;

	uart_gpio = getDefaultGPIO();
	uart_gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	uart_gpio.Mode = GPIO_MODE_AF_PP;
	uart_gpio.Alternate = GPIO_AF7_USART2;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &uart_gpio);
	HAL_UART_Init(&uart);
}

void UARTWrapper::release() {
	HAL_UART_DeInit(&uart);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
}


void UARTWrapper::send(std::string msg) {
	HAL_UART_Transmit(&uart, reinterpret_cast<uint8_t*>(const_cast<char*>(msg.c_str())), msg.length(), DEFAULT_TIMEOUT_MS);
}

std::string UARTWrapper::receive() {
	return "";
}

/** @} */
