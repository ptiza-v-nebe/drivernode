/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */
#include "hal/util.h"
#include "scheduler/Scheduler.h"

int main(void) {
	setupHardware();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpioa = getDefaultGPIO();
	gpioa.Pin = GPIO_PIN_5;
	gpioa.Mode = GPIO_MODE_OUTPUT_PP;

	HAL_GPIO_Init(GPIOA, &gpioa);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	schedule_repeating_task([](){
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}, 500);

	start_scheduler();

	for (;;);
}
