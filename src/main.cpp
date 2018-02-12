/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */
#include "hal/util.h"
#include "scheduler/Scheduler.h"
#include "serial/UARTWrapper.h"

int main(void) {
	setupHardware();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpioa = getDefaultGPIO();
	gpioa.Pin = GPIO_PIN_5;
	gpioa.Mode = GPIO_MODE_OUTPUT_PP;

	HAL_GPIO_Init(GPIOA, &gpioa);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	schedule_repeating_task([](){
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}, 500);

	UARTWrapper uartWrapper;
	schedule_repeating_task([&uartWrapper](){
		uartWrapper.send("Hello World - From the Nucleo :)\r\n");
	}, 1000, 250);

	start_scheduler();

	for (;;);
}
