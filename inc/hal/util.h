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

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"

void setupHardware();

GPIO_InitTypeDef getDefaultGPIO();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NUCLEO_HAL_UTIL_H_ */
/** @} */
