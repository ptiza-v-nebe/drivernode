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
#include "position/PositionParameterCalibration.h"
#include "hal/HALManager.h"
#include "position/PositionManager.h"

#include "serial/messages/all.h"
#include "util/util.h"

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

    HALManager& hal = HALManager::getInstance();

#ifdef CALIBRATION
    CalibrationMessageDispatcherFactory factory(hal.getLeftEncoder(), hal.getRightEncoder());
#else
#ifdef HUMAN_MODE
    HumanMessageDispatcherFactory factory;
#else
    ODROIDMessageDispatcherFactory factory;
#endif /*HUMAN_MODE*/
#endif /*CALIBRATION*/
    MessageDispatcher& dispatcher = factory.getMessageDispatcher();

    PositionManager pm(hal.getLeftEncoder(), hal.getRightEncoder());

    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////
    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&pm](ResetOdometryMessage rom) {
                pm.reset(rom.getPosition(), rom.getHeading());
            });

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////
#ifndef CALIBRATION
    schedule_repeating_task([&pm]() {
        pm.update();
    }, 5);
#endif

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

    /*Encoder& left = hal.getLeftEncoder();
     Encoder& right = hal.getRightEncoder();

     schedule_repeating_task([&left, &right](){
     printf("Links: %d, Rechts: %d\r\n", left.getTick(), right.getTick());
     }, 500);*/

    schedule_repeating_task(
            [&pm]() {
                printf("Position: (%d, %d), Facing %.2f degrees.\r\n",
                        pm.getPosition().x, pm.getPosition().y,
                        pm.getHeading().getAngleInDegrees());
            }, 800);

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
