/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */
#ifndef NUCLEO_HAL_UTIL_H_
#define NUCLEO_HAL_UTIL_H_

#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"

void setupHardware();

GPIO_InitTypeDef getDefaultGPIO();

#endif /* NUCLEO_HAL_UTIL_H_ */
/** @} */
