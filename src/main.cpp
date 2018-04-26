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

#include <control/HoneyControl.h>
#include <control/StartPinInitializer.h>

#include <functional>

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
    DriverFSM driverFSM(hal.getLeftMotor(), hal.getRightMotor(), pm, dispatcher,
            backwardVision);
    StartPinInitializer startPinInit(hal.getStarterPin());

#ifdef SMALL_ROBOT
    HoneyControl honeyControl(hal.getServoLeft(), hal.getServoRight());

    MainFSMContext mainFSM(dispatcher, {&driverFSM}, {&honeyControl,
                &startPinInit}, {&pm});
#endif
#ifdef BIG_ROBOT
    MainFSMContext mainFSM(dispatcher, {&driverFSM}, {&startPinInit},
            {   &pm});
#endif

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

    hal.getShootingBLDC().enable();
    dispatcher.registerMessageHandler<ShootingMechanismMessage>(
            [&hal](const ShootingMechanismMessage& smm) {
                if(smm.getCommand() == ShootingCommand::ENABLE) {
                    hal.getShootingBLDC().start();
                } else {
                    hal.getShootingBLDC().stop();
                }
            });

    dispatcher.registerMessageHandler<ServoControlMessage>(
            [&honeyControl, &dispatcher](const ServoControlMessage& scm) {
                honeyControl.processCommand(scm);
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

    schedule_repeating_task(
            [&hal, &backwardVision]() {
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
#if 0

    schedule_repeating_task([&hal, &stop]() {
                uint16_t d1 = hal.getSRF08s()[0].getRange();
                uint16_t d2 = hal.getSRF08s()[1].getRange();

                if(stop) {
                    if(d1 > 30 && d2 > 30) {
                        stop = false;
                    }
                } else {
                    if(d1 < 20 || d2 < 20) {
                        stop = true;
                    }
                }

                hal.getSRF08s()[0].startRanging();
                hal.getSRF08s()[1].startRanging();
            }, 100);
#endif

#if 1
    schedule_repeating_task([&]() {
        static InputPin& front = hal.getFrontSwitch();
        static bool stopped = false;
        if(front.isOn() && !stopped) {
            hal.getLeftMotor().stop();
            hal.getRightMotor().stop();
            dispatcher.sendMessage(StatusMessage(Status::DRIVER_FINISHED));

            stopped = true;
        } else if (front.isOff() && stopped) {
            stopped = false;
        }
    }, 50);
#endif

#if 0
    // Sensor Test
    schedule_repeating_task(
            [&hal]() {
                printf("\033[2J");
                printf("SENSOR TEST\r\n\n");
                printf("Encoders: Left %ld, Right %ld\r\n",
                        hal.getLeftEncoder().getTick(),
                        hal.getRightEncoder().getTick());
                printf("Starter Pin: %s\r\n", hal.getStarterPin().isOn() ? "INSERTED" : "REMOVED");
                printf("Ultrasonic Sensors: %d cm, %d cm\r\n",
                        hal.getSRF08s()[0].getRange(),
                        hal.getSRF08s()[1].getRange());
#ifdef BIG_ROBOT
                printf("Scara: Encoder %ld, EndStop: %s\r\n",
                        hal.getScaraHardware().getLiftEncoder().getTick(),
                        hal.getScaraHardware().getEndStop().isOn() ? "PRESSED" : "RELEASED");
#endif
#ifdef SMALL_ROBOT
                printf("Front Switch: %s\r\n", hal.getFrontSwitch().isOn() ? "PRESSED" : "RELEASED");
#endif
                hal.getSRF08s()[0].startRanging();
                hal.getSRF08s()[1].startRanging();
            }, 500);

    //Actor Test
    hal.getLeftMotor().enable();
    hal.getRightMotor().enable();

#ifdef BIG_ROBOT
    hal.getScaraHardware().getLiftMotor().enable();
    hal.getScaraHardware().getPump().enable();
    hal.getScaraHardware().getValve().enable();
    hal.getScaraHardware().getStoragePumps()[0].enable();
    hal.getScaraHardware().getStoragePumps()[1].enable();
    hal.getScaraHardware().getStoragePumps()[2].enable();

    hal.getScaraHardware().getArmServos()[0].enable();
    hal.getScaraHardware().getArmServos()[1].enable();
    hal.getScaraHardware().getArmServos()[2].enable();
    hal.getScaraHardware().getArmServos()[3].enable();

    hal.getScaraHardware().getArmServos()[0].setRPM(15);
    hal.getScaraHardware().getArmServos()[1].setRPM(15);
    hal.getScaraHardware().getArmServos()[2].setRPM(15);
    hal.getScaraHardware().getArmServos()[3].setRPM(15);
#endif
#ifdef SMALL_ROBOT
    hal.getShootingBLDC().enable();
    hal.getServoLeft().enable();
    hal.getServoRight().enable();
#endif
    schedule_repeating_task([&hal]() {
                hal.getLeftMotor().setSpeed(1000);
                hal.getRightMotor().setSpeed(1000);
                hal.getErrorLED().setOn();

#ifdef BIG_ROBOT
                hal.getScaraHardware().getLiftMotor().setSpeed(500);
                hal.getScaraHardware().getPump().setOff();
                hal.getScaraHardware().getValve().setOff();
                hal.getScaraHardware().getStoragePumps()[0].setOn();
                hal.getScaraHardware().getStoragePumps()[1].setOff();
                hal.getScaraHardware().getStoragePumps()[2].setOn();

                hal.getScaraHardware().getArmServos()[0].moveTo(150_deg);
                hal.getScaraHardware().getArmServos()[1].moveTo(150_deg);
                hal.getScaraHardware().getArmServos()[2].moveTo(60_deg);
                hal.getScaraHardware().getArmServos()[3].moveTo(150_deg);
#endif
#ifdef SMALL_ROBOT
                hal.getShootingBLDC().start();
                hal.getServoLeft().moveTo(150_deg);
                hal.getServoRight().moveTo(100_deg);
#endif
            }, 5000);
    schedule_repeating_task([&hal]() {
                hal.getLeftMotor().setSpeed(-1000);
                hal.getRightMotor().setSpeed(-1000);
                hal.getErrorLED().setOff();

#ifdef BIG_ROBOT
                hal.getScaraHardware().getLiftMotor().setSpeed(-500);
                hal.getScaraHardware().getPump().setOn();
                hal.getScaraHardware().getValve().setOn();
                hal.getScaraHardware().getStoragePumps()[0].setOff();
                hal.getScaraHardware().getStoragePumps()[1].setOn();
                hal.getScaraHardware().getStoragePumps()[2].setOff();

                hal.getScaraHardware().getArmServos()[0].moveTo(100_deg);
                hal.getScaraHardware().getArmServos()[1].moveTo(100_deg);
                hal.getScaraHardware().getArmServos()[2].moveTo(110_deg);
                hal.getScaraHardware().getArmServos()[3].moveTo(50_deg);
#endif
#ifdef SMALL_ROBOT
                hal.getShootingBLDC().stop();
                hal.getServoLeft().moveTo(100_deg);
                hal.getServoRight().moveTo(150_deg);
#endif
            }, 5000, 2500);

    dispatcher.registerMessageHandler<StopMessage>([&hal](StopMessage) {
                hal.disableAllActors();
            });
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
