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
#include "hal/HALManagerBigRobot.h"

#include "serial/messages/SetSpeedMessage.h"
#include "hal/DynamixelUART.h"

int main(void) {
    setupHardware();
#ifdef BLINK_LED
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpioa = getDefaultGPIO();
    gpioa.Pin = GPIO_PIN_5;
    gpioa.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(GPIOA, &gpioa);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    schedule_repeating_task([]() {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }, 500);
#endif

#ifdef HUMAN_MODE
    HumanMessageDispatcherFactory factory;
#else
    ODROIDMessageDispatcherFactory factory;
#endif
    MessageDispatcher& dispatcher = factory.getMessageDispatcher();


    /*Encoder& left = HALManagerBigRobot::getInstance().getLeftEncoder();
    schedule_repeating_task([&left](){
        printf("Encoder tick delta is: %d\r\n", left.getTickAndReset());
    }, 1000, 250);*/


#if 0
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_DAC1_CLK_ENABLE();
    DAC_HandleTypeDef dac;
    dac.Instance = DAC1;

    GPIO_InitTypeDef gpio_dac = getDefaultGPIO();
    gpio_dac.Mode = GPIO_MODE_ANALOG;
    gpio_dac.Pin = GPIO_PIN_4;

    HAL_DAC_Init(&dac);
    HAL_GPIO_Init(GPIOA, &gpio_dac);
    //HAL_DAC_ConfigChannel(&dac, &ch1, DAC_CHANNEL_1);
    HAL_DAC_Start(&dac, DAC_CHANNEL_1);

    HAL_DAC_SetValue(&dac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);


    dispatcher.registerMessageHandler<SetSpeedMessage>([&dac](SetSpeedMessage ssm) {
        HAL_DAC_SetValue(&dac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ssm.getSpeedLeft());
    });
#endif

    DynamixelUART dynamixel;

    // ping
    //uint8_t checksum = ~(5 + 2 + 0x01);
    //uint8_t msg[] = {0xFF, 0xFF, 5, 2, 0x01, checksum};
    //int size = 6;

    // turn on led
    //uint8_t checksum = ~(5 + 4 + 0x03 + 25 + 1);
    //uint8_t msg[] = {0xFF, 0xFF, 5, 4, 0x03, 25, 1, checksum};
    //int size = 8;

    uint8_t checksum = ~(5 + 4 + 0x02 + 42 + 1);
    uint8_t msg[] = {0xFF, 0xFF, 5, 4, 0x02, 42, 1, checksum};
    int size = 8;

    printf("Sending \"");
    printBytes(msg, size);
    printf("\" to Dynamixel...\r\n");
    dynamixel.send(msg, size);

    uint8_t buffer[7] = {0};
    int result = dynamixel.receive(buffer, 7);
    printf("Result is %d\r\n", result);
    printf("Got response \"");
    printBytes(buffer, 7);
    printf("\" from Dynamixel...\r\n");
    printf("Temp is %d\r\n", buffer[5]);

    start_scheduler();

    for (;;)
        ;
}
