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

#include "hal/PWM.h"
#include "hal/OutputPin.h"
#include "hal/InputPin.h"
#include "hal/ShootingBLDC.h"
#include "hal/DynamixelAX12A.h"
#include "error.h"
#include "scara/ScaraLift.h"

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

    HALManager& hal = HALManager::getInstance();
    hal.enableISRs();

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

    /*ScaraLift scaraLift(hal.getScaraLiftMotor(), hal.getScaraLiftEncoder());
    scaraLift.initialize();

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&scaraLift](SetSpeedMessage ssm) {
                scaraLift.moveTo(ssm.getSpeedLeft());
            });

    dispatcher.registerMessageHandler<StopMessage>([&hal](StopMessage) {
        hal.getScaraLiftMotor().disableAndStop();
    });*/

    /*Motor& scaraLift = hal.getScaraLiftMotor();
     scaraLift.enable();

     dispatcher.registerMessageHandler<SetSpeedMessage>(
     [&scaraLift](SetSpeedMessage ssm) {
     scaraLift.setSpeed(ssm.getSpeedLeft());
     });
     dispatcher.registerMessageHandler<StopMessage>([&scaraLift](StopMessage) {
     scaraLift.disableAndStop();
     });
     dispatcher.registerMessageHandler<SimpleDriveMessage>([&scaraLift](SimpleDriveMessage) {
     scaraLift.enable();
     });*/

    schedule_repeating_task([&dispatcher, &pm]() {
        PositionMessage posM(pm.getPosition(), pm.getHeading());
        dispatcher.sendMessage(posM);
    }, 500);


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
