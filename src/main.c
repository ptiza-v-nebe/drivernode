/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */
#include "hal/util.h"

void HAL_SYSTICK_Callback(void){
    static int ms = 0;

    ms++;
    if(ms == 500){
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        ms = 0;
    }
}

int main(void) {
	setupHardware();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpioa = getDefaultGPIO();
	gpioa.Pin = GPIO_PIN_5;
	gpioa.Mode = GPIO_MODE_OUTPUT_PP;

	HAL_GPIO_Init(GPIOA, &gpioa);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	for (;;);
}
