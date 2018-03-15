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

    //HALManager& hal = HALManager::getInstance();

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
    DynamixelUART dynamixel;
    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&dynamixel](SetSpeedMessage ssm) {
        // turn on led
        //uint8_t checksum = ~(5 + 4 + 0x03 + 25 + 1);
        //uint8_t msg[] = {0xFF, 0xFF, 5, 4, 0x03, 25, 1, checksum};
        //int size = 8;



                uint8_t id = 7;
                uint8_t checksum = ~(id + 4 + 0x03 + 25 + ssm.getSpeedLeft());
                uint8_t msg[] = {0xFF, 0xFF, id, 4, 0x03, 25, static_cast<uint8_t>(ssm.getSpeedLeft()), checksum};
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

    int id = 7;

    printf("Sending ping \r\n");
    dynamixel.ping(id);

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&dynamixel, id](SetSpeedMessage ssm) {
                //printf("Setting LED to %d \r\n", ssm.getSpeedLeft());
                //dynamixel.writeByte(id, 25, ssm.getSpeedLeft());

                uint16_t speed = ssm.getSpeedLeft();
                printf("Setting GoalPosition to %d \r\n", speed);

                dynamixel.writeWord(7, 30, speed);
                dynamixel.writeWord(2, 30, speed);
                dynamixel.writeWord(5, 30, speed);
            });
#endif

    // ////////////////////////////////////////////
    // Start Scheduler and execute Tasks
    // ////////////////////////////////////////////
    start_scheduler();

    for (;;)
        ;
}
