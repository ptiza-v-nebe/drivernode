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
#include "util/util.h"

#include "hal/DynamixelMX12W.h"

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

    // ////////////////////////////////////////////
    // BEGIN TEST AREA
    // ////////////////////////////////////////////

    DynamixelCOM dynamixel;

    DynamixelMX12W mx12w(13, dynamixel);

    /*int id = 254;

     printf("Searching Dynamixel...\r\n");
     for(int i = 0; i < 254; i++) {
     if(dynamixel.ping(i) == 0){
     printf("Found Dynamixel with ID %d\r\n", i);
     id = i;
     break;
     }
     }*/

    /*printf("Switching to Wheel Mode...\r\n");
     dynamixel.writeWord(id, 32, 0); // speed 0
     dynamixel.writeWord(id, 6, 0); // cw limit 0
     dynamixel.writeWord(id, 8, 0); // ccw limit 0
     // both limits 0 --> wheel mode*/

    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&mx12w](ResetOdometryMessage rom) {
                float rpm = rom.getHeading() * 180.0 / 3.141592;
                mx12w.setRPM(rpm);
                printf("Setting RPM to %.2f \r\n", rpm);
            });

    dispatcher.registerMessageHandler<StopMessage>([&mx12w](StopMessage) {
        mx12w.disableAndStop();
    });

    dispatcher.registerMessageHandler<SetSpeedMessage>([&mx12w](SetSpeedMessage) {
        mx12w.enable();
    });

    // ////////////////////////////////////////////
    // END TEST AREA
    // ////////////////////////////////////////////

    // ////////////////////////////////////////////
    // Start Scheduler and execute Tasks
    // ////////////////////////////////////////////
    start_scheduler();

    for (;;)
        ;
}
