/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_DYNAMIXELUART_H_
#define HAL_DYNAMIXELUART_H_

#include <stm32l4xx.h>

class DynamixelUART {
private:
    UART_HandleTypeDef uart; ///< low level uart handle
    GPIO_InitTypeDef uart_gpio; ///< low level gpio handle
public:
    DynamixelUART();
    void send(const uint8_t* buffer, const int size);
    int receive(uint8_t* buffer, const int size);
private:
    void init();
};

#endif /* HAL_DYNAMIXELUART_H_ */
/** @} */
