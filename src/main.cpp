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

    Motor& left = hal.getLeftMotor();
    Motor& right = hal.getRightMotor();

    left.enable();
    right.enable();

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&left, &right](SetSpeedMessage ssm) {
                left.setSpeed(ssm.getSpeedLeft());
                right.setSpeed(ssm.getSpeedRight());
            });
    dispatcher.registerMessageHandler<StopMessage>(
            [&left, &right](StopMessage) {
                left.stop();
                right.stop();
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
