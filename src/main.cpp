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
#include "error.h"
#include "serial/messages/version.h"
#include "serial/messages/all.h"

#include <functional>

#include <stm32l4xx.h>

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

    HALManager& hal = HALManager::getInstance();

    bool backwardVision = false;

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
    DriverFSM driverFSM(hal.getLeftMotor(), hal.getRightMotor(), pm, dispatcher,
            backwardVision);


    MainFSMContext mainFSM(dispatcher, {&driverFSM}, {}, {&pm});


    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////
    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&pm](ResetOdometryMessage rom) {
                pm.reset(rom.getPosition(), rom.getHeading());
            });

    dispatcher.registerMessageHandler<StopMessage>(
            [&driverFSM](const StopMessage&) {
                driverFSM.stop();
            });

    dispatcher.registerMessageHandler<ControlledDriveMessage>(
            [&driverFSM](const ControlledDriveMessage& cdm) {
                driverFSM.setReferencePosition(cdm.getPosition());
                driverFSM.setDriveDirection(cdm.getDirection());
                driverFSM.setDriveSpeed(cdm.getSpeed());
                driverFSM.setDriveAccuracy(cdm.getAccuracy());
                driverFSM.newPosition();
            });

    dispatcher.registerMessageHandler<ControlledTurnMessage>(
            [&driverFSM](const ControlledTurnMessage& ctm) {
                driverFSM.setReferenceAngle(ctm.getTargetHeading());
                driverFSM.setDriveDirection(DriveDirection::FORWARD);
                driverFSM.newAngle();
            });

    dispatcher.registerMessageHandler<SetSpeedMessage>(
            [&hal](const SetSpeedMessage& ssm) {
                hal.getLeftMotor().enable();
                hal.getRightMotor().enable();
                hal.getLeftMotor().setSpeed(ssm.getSpeedLeft());
                hal.getRightMotor().setSpeed(ssm.getSpeedRight());
    });

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////
#ifndef TEST_ALL

#ifndef HUMAN_MODE
    schedule_repeating_task(
            [&dispatcher, &pm]() {
                dispatcher.sendMessage(PositionMessage(pm.getPosition(), pm.getHeading()));
            }, 100);
#endif /* HUMAN_MODE */

    schedule_repeating_task([&mainFSM]() {
        mainFSM.tick();
    }, 10);
#endif

#ifdef BLINK_LED
    schedule_repeating_task([&hal]() {
        hal.getStatusLED().toggle();
    }, 250);
#endif


    // ////////////////////////////////////////////
    // BEGIN TEST AREA
    // ////////////////////////////////////////////
#ifdef TEST_ALL
    // Sensor Test
    schedule_repeating_task(
            [&hal]() {
                printf("\033[2J");
                printf("SENSOR TEST\r\n\n");
                printf("Encoders: Left %ld, Right %ld\r\n",
                        hal.getLeftEncoder().getTick(),
                        hal.getRightEncoder().getTick());
                printf("Currents: Left %d, Right %d\r\n",
                		hal.getLeftCurrentSensor().getShuntV(),
						hal.getRightCurrentSensor().getShuntV());

            }, 250);
#if 1
    //Actor Test
    hal.getLeftMotor().enable();
    hal.getRightMotor().enable();

    schedule_repeating_task([&hal]() {
                hal.getLeftMotor().setSpeed(1000);
                hal.getRightMotor().setSpeed(1000);
                hal.getErrorLED().setOn();

            }, 5000);
    schedule_repeating_task([&hal]() {
                hal.getLeftMotor().setSpeed(-1000);
                hal.getRightMotor().setSpeed(-1000);
                hal.getErrorLED().setOff();
            }, 5000, 2500);

    dispatcher.registerMessageHandler<StopMessage>([&hal](StopMessage) {
                hal.disableAllActors();
            });
#endif
#endif

    // ////////////////////////////////////////////
    // END TEST AREA
    // ////////////////////////////////////////////

    // ////////////////////////////////////////////
    // Start Scheduler and execute Tasks
    // ////////////////////////////////////////////
#endif /*CALIBRATION*/
    start_scheduler();

    for (;;){
        uint16_t leftCurrent = hal.getLeftCurrentSensor().getShuntV();
        printf("left current %d\n\r", leftCurrent);
        HAL_Delay(250);
    }
}
