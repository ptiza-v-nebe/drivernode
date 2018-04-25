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
#include "serial/messages/all.h"
#include "scara/ScaraLift.h"

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
    DriverFSM driverFSM(hal.getLeftMotor(), hal.getRightMotor(), pm,
            dispatcher, backwardVision);

    MainFSMContext mainFSM(dispatcher, { &driverFSM }, { }, { &pm });

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
#ifndef HUMAN_MODE
    schedule_repeating_task(
            [&dispatcher, &pm]() {
                dispatcher.sendMessage(PositionMessage(pm.getPosition(), pm.getHeading()));
            }, 100);
#endif /* HUMAN_MODE */

    schedule_repeating_task([&mainFSM]() {
        mainFSM.tick();
    }, 10);

#ifdef BLINK_LED
    schedule_repeating_task([&hal]() {
        hal.getStatusLED().toggle();
    }, 250);
#endif

	schedule_repeating_task([&hal, &backwardVision]() {
		uint16_t d1 = hal.getSRF08s()[0].getRange();
		uint16_t d2 = hal.getSRF08s()[1].getRange();

		if(backwardVision) {
			if(d1 > ULTRASONIC_HIGHERTHRESHOLD && d2 > ULTRASONIC_HIGHERTHRESHOLD) {
				backwardVision = false;
			}
		} else {
			if(d1 < ULTRASONIC_LOWERTHRESHOLD || d2 < ULTRASONIC_LOWERTHRESHOLD) {
				backwardVision = true;
			}
		}

		hal.getSRF08s()[0].startRanging();
		hal.getSRF08s()[1].startRanging();
	}, 100);

    // ////////////////////////////////////////////
    // BEGIN TEST AREA
    // ////////////////////////////////////////////
#if 1

#endif
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
