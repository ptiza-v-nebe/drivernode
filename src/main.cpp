/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */
#include "config.h"

#include "hal/util.h"
#include "scheduler/Scheduler.h"
#include "serial/MessageDispatcherFactory.h"

#include "srf_08/srf08.h"

static void InitI2C(I2C_HandleTypeDef& hi2c1);

int main(void) {
	setupHardware();

#ifdef BLINK_LED
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;

	GPIO_InitTypeDef gpioa = getDefaultGPIO();
	gpioa.Pin = GPIO_PIN_5;
	gpioa.Mode = GPIO_MODE_OUTPUT_PP;

	HAL_GPIO_Init(GPIOA, &gpioa);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	schedule_repeating_task([]() {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}, 100);
#endif

#ifdef HUMAN_MODE
	HumanMessageDispatcherFactory factory;
#else
	ODROIDMessageDispatcherFactory factory;
#endif
	MessageDispatcher& dispatcher = factory.getMessageDispatcher();
	/* SRF08 Messung Funktionen*/
	I2C_HandleTypeDef i2c;
	InitI2C(i2c);

	SRF08 srf08(&i2c, 0xE2);
	schedule_repeating_task([&srf08]() {
		uint16_t cm = srf08.getRange();
		if(cm < 10){
			printf("Warnung %d cm\r\n", cm);
		}
//		printf("Range is %d cm\r\n", cm);
//		printf("Measuring...\r\n");
		srf08.startRanging();
		srf08.rangingFinished();
	}, 100);

	start_scheduler();

	for (;;)
		;
}

/* I2C1 init function */
void InitI2C(I2C_HandleTypeDef& hi2c1) {
	hi2c1.Instance = I2C2;
	hi2c1.Init.Timing = 0x10909CEC;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	__HAL_RCC_I2C2_CLK_ENABLE()
	;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		//TODO: error handling
		while (1)
			;
	}

	GPIO_InitTypeDef i2cGPIO = getDefaultGPIO();
	i2cGPIO.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	i2cGPIO.Mode = GPIO_MODE_AF_OD;
	i2cGPIO.Alternate = GPIO_AF4_I2C2;

	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	HAL_GPIO_Init(GPIOB, &i2cGPIO);

}

