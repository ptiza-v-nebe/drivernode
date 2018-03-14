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
#include "hal/HALManager.h"

#include "serial/messages/all.h"
#include "hal/DynamixelCOM.h"
#include "util/util.h"
#include "hal/FaulhaberBLDC.h"

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

#ifdef HUMAN_MODE
    HumanMessageDispatcherFactory factory;
#else
    ODROIDMessageDispatcherFactory factory;
#endif
    MessageDispatcher& dispatcher = factory.getMessageDispatcher();

    HALManager& hal = HALManager::getInstance();

    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////

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
    }, 500);
#endif

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
#if 0
    Motor& motor = HALManager::getInstance().getLeftMotor();
    motor.enable();
    motor.setSpeed(2048);

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&motor](SetSpeedMessage ssm) {
                motor.setSpeed(ssm.getSpeedLeft());
            });

#endif

#if 0
    DynamixelUART dynamixel;
    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&dynamixel](SetSpeedMessage) {
        // turn on led
        //uint8_t checksum = ~(5 + 4 + 0x03 + 25 + 1);
        //uint8_t msg[] = {0xFF, 0xFF, 5, 4, 0x03, 25, 1, checksum};
        //int size = 8;



                uint8_t id = 5;
                uint8_t checksum = ~(id + 4 + 0x03 + 25 + 1);
                uint8_t msg[] = {0xFF, 0xFF, id, 4, 0x03, 25, 1, checksum};
                constexpr int size = 8;
                uint8_t answer[size] = {0};

                dynamixel.send(msg, size);
                int result = dynamixel.receive(answer, 6);

                printf("Sent ");
                printBytes(msg, size);
                printf(" and got (%d) ", result);
                printBytes(answer, size);
                printf("\r\n");
            });

#endif

#if 1
    DynamixelCOM dynamixel;

    int id = 5;

    printf("Sending ping \r\n");
    dynamixel.ping(id);

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&dynamixel, id](SetSpeedMessage ssm) {
                printf("Setting LED to %d \r\n", ssm.getSpeedLeft());
                dynamixel.writeByte(id, 25, ssm.getSpeedLeft());

                //uint16_t speed = ssm.getSpeedLeft();
                //printf("Setting GoalPosition to %d \r\n", speed);

                //uint8_t data[] = {};
                //dynamixel.write();
                //dynamixel.writeByte(id, 25, ssm.getSpeedLeft());
            });
#endif

    // ////////////////////////////////////////////
    // Start Scheduler and execute Tasks
    // ////////////////////////////////////////////
    start_scheduler();

    for (;;)
        ;
}
