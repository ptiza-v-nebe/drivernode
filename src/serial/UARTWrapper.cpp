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
#include "error.h"

// ////////////////////////////////////////////////////////
// ISRs and other C-Stuff
// ////////////////////////////////////////////////////////
extern "C" {
/**
 * ISR for USART2
 */
void UART4_IRQHandler() {
    if (UART5->ISR & USART_ISR_RXNE) {
        // byte was received
        UARTWrapper::getInstance().handleByte(UART5->RDR);
    } else {
        ERROR("Unexpected IRQ on UART5!");
    }
}

#ifdef HUMAN_MODE
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
#endif

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
        uart {}, uart_gpio {}, receiveHandler(nullptr) {
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
    uart.Instance = UART5;

    uart.Init.BaudRate = 115200;
    uart.Init.Parity = UART_PARITY_NONE;
    uart.Init.Mode = UART_MODE_TX_RX;
    uart.Init.StopBits = UART_STOPBITS_1;
    uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart.Init.WordLength = UART_WORDLENGTH_8B;
    uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_ENABLED;
    uart.Init.OverSampling = UART_OVERSAMPLING_16;

    uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_RCC_UART5_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    HAL_UART_Init(&uart);

    uint8_t unusedBuffer;
    // Use receive IT function but we are actually handling the interrupt ourselves
    HAL_UART_Receive_IT(&uart, &unusedBuffer, 1);
    HAL_NVIC_SetPriority(UART5_IRQn, UART_PREMPTION_PRIO, UART_SUB_PRIO);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
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
