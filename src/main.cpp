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
#include "driving/DriverFSM.h"
#include "driving/DrivingParameterMeasurement.h"
#include "constants.h"

#include "serial/messages/all.h"
#include "util/util.h"
#include "control/MainFSM.h"

#include "hal/PWM.h"
#include "hal/OutputPin.h"
#include "hal/InputPin.h"
#include "hal/ShootingBLDC.h"
#include "hal/DynamixelAX12A.h"
#include "error.h"
#include "serial/messages/version.h"
#include "scara/ScaraLift.h"
#include "scara/Scara.h"

#include <stm32l4xx.h>

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOD_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOH_CLK_ENABLE()
    ;

    HALManager& hal = HALManager::getInstance();
    hal.enableISRs();

#ifdef CALIBRATION
    CalibrationMessageDispatcherFactory factory(hal.getLeftEncoder(), hal.getRightEncoder());
#elif defined(DRIVERMEASUREMENT)
    PositionManager pm(hal.getLeftEncoder(), hal.getRightEncoder());
    MeasurementMessageDispatcherFactory factory(pm, hal.getLeftMotor(), hal.getRightMotor());
#else
#ifdef HUMAN_MODE
    HumanMessageDispatcherFactory factory;
#else
    ODROIDMessageDispatcherFactory factory;
#endif /*HUMAN_MODE*/

    MessageDispatcher& dispatcher = factory.getMessageDispatcher();
    PositionManager pm(hal.getLeftEncoder(), hal.getRightEncoder());
    DriverFSM driverFSM(hal.getLeftMotor(), hal.getRightMotor(), pm);

    //MainFSMContext mainFSM(dispatcher, { }, { }, { &pm });

    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////
    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&pm](ResetOdometryMessage rom) {
                pm.reset(rom.getPosition(), rom.getHeading());
            });

    /*dispatcher.registerMessageHandler<ControlledDriveMessage>(
     [&driverFSM](ControlledDriveMessage cdm) {
     driverFSM.setTargetPosition(cdm.getPosition());
     driverFSM.setDriveSpeed(cdm.getSpeed());
     driverFSM.newTargetPosition();

     });*/

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////
    /*schedule_repeating_task([&mainFSM]() {
     mainFSM.tick();
     }, 10);*/

#ifdef BLINK_LED
    schedule_repeating_task([&hal]() {
        hal.getStatusLED().toggle();
    }, 250);
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

    Scara scara(hal.getScaraHardware());
    scara.initialize();

    schedule_repeating_task([&]() { //here no pauses!
        scara.tick();
    }, 10); //, 10) each 10 milliseconds run this code

    dispatcher.registerMessageHandler<ScaraActionMessage>(
            [&](ScaraActionMessage sam) {
    			scara.commandReceived(sam);
                //scara.moveToCartesian(sam.getX(), sam.getY(), sam.getPhi().getAngleInRadian(), sam.getStorageSpace());
                //StorageSpace s = StorageSpace::INNER_3;
                //static_cast<int>(s);//enum as int
            });
    dispatcher.registerMessageHandler<BasicScaraMessage>(
            [&](BasicScaraMessage bsm) {
                if(bsm.getScaraCommand() == ScaraCommand::CANCEL) {
                    // TODO
                } else if (bsm.getScaraCommand() == ScaraCommand::PARK) {
                    // TODO
                }
            });

    // ////////////////////////////////////////////
    // END TEST AREA
    // ////////////////////////////////////////////

    // ////////////////////////////////////////////
    // Start Scheduler and execute Tasks
    // ////////////////////////////////////////////
#endif /*CALIBRATION*/
    start_scheduler();

    for (;;)
        ;
}
