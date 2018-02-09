/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32l4xx.h"
#include "stm32l4xx_nucleo.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_rcc.h"

TIM_HandleTypeDef timer;

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&timer);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void setupCLK(void) {
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
	// Final state: MSI: 4 MHz, PLL-out: 80MHz, SYSCLK: PLL-out
}

int main(void) {
	setupCLK();

	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();

	GPIO_InitTypeDef gpioa;
	gpioa.Pin = GPIO_PIN_5;
	gpioa.Mode = GPIO_MODE_OUTPUT_PP;
	gpioa.Pull = GPIO_NOPULL;
	gpioa.Speed = GPIO_SPEED_MEDIUM;

	timer.Instance = TIM2;
	timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timer.Init.Prescaler = 1 - 1;
	timer.Init.Period = 500 - 1;
	timer.Init.RepetitionCounter = 0;

	HAL_TIM_Base_Init(&timer);

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
	HAL_GPIO_Init(GPIOA, &gpioa);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	uint32_t freq = HAL_RCC_GetSysClockFreq();
	if(freq ==  80000000)	{
		HAL_TIM_Base_Start_IT(&timer);
	}

	for (;;);
}
