# Microcontroller software for the Eurobot competition
This repository contains the sourcecode for the nucleo microcontroller on the robot.

## Information:
* Programming Language: C++
* Language Standard: C++11 (?)
* Compiler: GCC 7 (?)

## Board:
* STM32 Nucleo-64 F476RG
* Cortex L4 (low power M4)
* 64 pins

## Git repository:
The repository does not contain everything neded to run the code.  
It requires a properly setup eclipse project with the CubHAL drivers set to be linked as an external static library. 

## Settings in the Driver:
stm32l4xx_hal_conf.h:
```
/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */
#define HAL_MODULE_ENABLED
//#define HAL_ADC_MODULE_ENABLED
//#define HAL_CAN_MODULE_ENABLED
// #define HAL_CAN_LEGACY_MODULE_ENABLED
//#define HAL_COMP_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
//#define HAL_CRC_MODULE_ENABLED
//#define HAL_CRYP_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
//#define HAL_DFSDM_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
//#define HAL_FIREWALL_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
//#define HAL_HCD_MODULE_ENABLED
//#define HAL_NAND_MODULE_ENABLED
//#define HAL_NOR_MODULE_ENABLED
//#define HAL_SRAM_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
//#define HAL_IRDA_MODULE_ENABLED
//#define HAL_IWDG_MODULE_ENABLED
//#define HAL_LCD_MODULE_ENABLED
//#define HAL_LPTIM_MODULE_ENABLED
//#define HAL_OPAMP_MODULE_ENABLED
//#define HAL_PCD_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
//#define HAL_QSPI_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
//#define HAL_RNG_MODULE_ENABLED
//#define HAL_RTC_MODULE_ENABLED
//#define HAL_SAI_MODULE_ENABLED
//#define HAL_SD_MODULE_ENABLED
//#define HAL_SMARTCARD_MODULE_ENABLED
//#define HAL_SMBUS_MODULE_ENABLED
//#define HAL_SPI_MODULE_ENABLED
//#define HAL_SWPMI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
//#define HAL_TSC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
//#define HAL_USART_MODULE_ENABLED
//#define HAL_WWDG_MODULE_ENABLED
```