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

#include "hal/PWM.h"
#include "hal/DynamixelAX12A.h"
#include "error.h"

int main(void) {
    setupHardware();

    // ////////////////////////////////////////////
    // Setup Objects
    // ////////////////////////////////////////////

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
#endif /*CALIBRATION*/
    MessageDispatcher& dispatcher = factory.getMessageDispatcher();

#ifndef CALIBRATION
#ifndef DRIVERMEASUREMENT
    PositionManager pm(hal.getLeftEncoder(), hal.getRightEncoder());
    DriverFSM driverFSM(hal.getLeftMotor(), hal.getRightMotor(), pm);
#endif
#endif

    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////
#ifndef CALIBRATION
    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&pm](ResetOdometryMessage rom) {
                pm.reset(rom.getPosition(), rom.getHeading());
            });

#ifndef DRIVERMEASUREMENT
    dispatcher.registerMessageHandler<ControlledDriveMessage>(
    		[&driverFSM](ControlledDriveMessage cdm) {
    			driverFSM.setTargetPosition(cdm.getPosition());
    			driverFSM.setDriveSpeed(cdm.getSpeed());
    			driverFSM.newTargetPosition();

    		});

    dispatcher.registerMessageHandler<SimpleDriveMessage>(
    		[&driverFSM](SimpleDriveMessage sdm) {

    		});
#endif
#endif

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////
#ifndef CALIBRATION
    schedule_repeating_task([&pm]() {
    	pm.update();
    }, 5);
#endif

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

    // ////////////////////////////////////////////
    // BEGIN TEST AREA
    // ////////////////////////////////////////////

#ifndef CALIBRATION
#ifndef DRIVERMEASUREMENT
    schedule_repeating_task([&pm, &driverFSM]() {
    	pm.update();
    	driverFSM.update();
    }, CONTROLLER_SAMPLING_TIME*1000);
#endif
#endif

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
