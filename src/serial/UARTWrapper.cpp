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
#include "hal/interupts.h"
#include "constants.h"

// ////////////////////////////////////////////////////////
// ISRs and other C-Stuff
// ////////////////////////////////////////////////////////
extern "C" {
/**
 * ISR for USART2
 */
void USART2_IRQHandler() {
    if (USART2->ISR & USART_ISR_RXNE) {
        // byte was received
        UARTWrapper::getInstance().handleByte(USART2->RDR);
    } else {
        // TODO: unexpected...error maybe?
    }
}

/**
 * Low-Level 'syscall' needed for printf.
 *
 * @param ch the character to send
 * @return always 0
 */
int __io_putchar(int ch) {
    UARTWrapper::getInstance().send(reinterpret_cast<uint8_t*>(&ch), 1);
    return 0;
}

} // extern "C"

// ////////////////////////////////////////////////////////
// Implementation
// ////////////////////////////////////////////////////////
/**
 * Singleton access to the UART Wrapper.
 *
 * @return the UARTWrapper (for USART2)
 */
UARTWrapper& UARTWrapper::getInstance() {
    static UARTWrapper instance; // Meyers singleton
    return instance;
}

/**
 * Constructs the UARTWrapper
 */
UARTWrapper::UARTWrapper() :
        receiveHandler(nullptr) {
    init();
}

/**
 * Handles receiving a byte.
 *
 * @param byte the byte that was received
 */
void UARTWrapper::handleByte(uint8_t byte) {
    if (receiveHandler) {
        receiveHandler->processByte(byte);
    }
}

/**
 * Change the receive handler that will be notified about incoming bytes.
 *
 * @param receiveHandler the new receive handler
 */
void UARTWrapper::setReceiveHandler(UARTReceiveHandler* receiveHandler) {
    this->receiveHandler = receiveHandler;
}

/**
 * Initialize the UART instance
 */
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

    uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    uart_gpio = getDefaultGPIO();
    uart_gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    uart_gpio.Mode = GPIO_MODE_AF_PP;
    uart_gpio.Alternate = GPIO_AF7_USART2;

    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_USART2_CLK_ENABLE()
    ;

    HAL_GPIO_Init(GPIOA, &uart_gpio);
    HAL_UART_Init(&uart);

    uint8_t unusedBuffer;
    // Use receive IT function but we are actually handling the interrupt ourselves
    HAL_UART_Receive_IT(&uart, &unusedBuffer, 1);
    HAL_NVIC_SetPriority(USART2_IRQn, UART_PREMPTION_PRIO, UART_SUB_PRIO);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * Send data via UART
 *
 * @param buffer the data to send
 * @param size   the size of the data
 */
void UARTWrapper::send(const uint8_t* buffer, const int size) {
    HAL_UART_Transmit(&uart, const_cast<uint8_t*>(buffer), size,
            DEFAULT_TIMEOUT_MS);
}
/** @} */
