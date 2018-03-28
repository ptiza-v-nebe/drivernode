/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include "hal/DynamixelUART.h"
#include "hal/util.h"
#include "constants.h"

DynamixelUART::DynamixelUART() {
    init();
}

/**
 * Send data via UART
 *
 * @param buffer the data to send
 * @param size   the size of the data
 */
void DynamixelUART::send(const uint8_t* buffer, const int size) {
    HAL_HalfDuplex_EnableTransmitter(&uart);
    HAL_UART_Transmit(&uart, const_cast<uint8_t*>(buffer), size,
            DEFAULT_TIMEOUT_MS);
    HAL_HalfDuplex_EnableReceiver(&uart);
}

/**
 * Receive data from UART
 *
 * @param buffer buffer for the data
 * @param size   the size of the buffer (= how many bytes to receive)
 */
int DynamixelUART::receive(uint8_t* buffer, const int size) {
    // assuming receiver is already enabled
    auto result = HAL_UART_Receive(&uart, buffer, size, DEFAULT_TIMEOUT_MS);
    return ((result == HAL_OK) ? 0 : -1);
}

/**
 * Initialize the UART instance
 */
void DynamixelUART::init() {
    uart.Instance = USART1;

    uart.Init.BaudRate = 500000;
    uart.Init.Parity = UART_PARITY_NONE;
    uart.Init.Mode = UART_MODE_TX_RX;
    uart.Init.StopBits = UART_STOPBITS_1;
    uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart.Init.WordLength = UART_WORDLENGTH_8B;
    uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_ENABLED;
    uart.Init.OverSampling = UART_OVERSAMPLING_8;

    uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    uart_gpio = getDefaultGPIO();
    uart_gpio.Pin = GPIO_PIN_9; // PA9: USART1_TX
    uart_gpio.Mode = GPIO_MODE_AF_OD;
    uart_gpio.Alternate = GPIO_AF7_USART1;
    uart_gpio.Speed = GPIO_SPEED_HIGH;
    uart_gpio.Pull = GPIO_NOPULL;

    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_USART1_CLK_ENABLE()
    ;

    HAL_GPIO_Init(GPIOA, &uart_gpio);
    HAL_HalfDuplex_Init(&uart);
    HAL_HalfDuplex_EnableReceiver(&uart);
}
/** @} */
