/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include "hal/util.h"

/**
 * Setup the internal clock to 80MHz using PLL.
 * Also sets Cache latency appropriately
 */
static void setupCLK(void) {
    // Reset RCC to known state
    HAL_RCC_DeInit();
    // Current state: MSI: 4 MHz, SYSCLK: MSI

    // Disable PLL
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

    // Wait for PLL to stop (PLLREADY cleared)
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY)){}

    // Modify PLL
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_MSI); // source = MSI
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, 0 << RCC_PLLCFGR_PLLM_Pos); // PLL_M = 1;
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, 40 << RCC_PLLCFGR_PLLN_Pos); // PLL_N = 40;
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, 0 << RCC_PLLCFGR_PLLR_Pos); // PLL_R = 2;

    // PLL-out = ((MSI / PLL_M) * PLL_M) / PLL_R = ((4 MHz / 1) * 40) / 2 = 80 MHz

    // Enable PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);

    // Enable PLL outputs
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);

    // Wait for PLL to become locked (PLLREADY set)
    while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY)){}

    // Adjust Flash latency for higher clock
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);

    // Check if Flash latency is set correctly
    if((READ_REG(FLASH->ACR) & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS){
        // Error
        while(1){}
    }

    // Select PLL as SYSCLK Source
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    // Update SystemCoreCloc Variable
    SystemCoreClockUpdate();
    HAL_RCC_GetSysClockFreq();
    // Final state: MSI: 4 MHz, PLL-out: 80MHz, SYSCLK: PLL-out
}

/**
 * Setup the Hardware.
 * This should be the first call in main()
 */
void setupHardware(){
    setupCLK();
    HAL_Init();
}

GPIO_InitTypeDef getDefaultGPIO(){
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = 0; // no pins
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Speed = GPIO_SPEED_MEDIUM;
    gpio.Pull = GPIO_NOPULL;
    gpio.Alternate = 0; // no AF

    return gpio;
}


/** @} */
