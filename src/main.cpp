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
#include "control/MainFSM.h"

#include "hal/PWM.h"
#include "hal/OutputPin.h"
#include "hal/InputPin.h"
#include "hal/ShootingBLDC.h"
#include "hal/DynamixelAX12A.h"
#include "error.h"
#include "serial/messages/version.h"
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

#ifdef CALIBRATION
    CalibrationMessageDispatcherFactory factory(hal.getLeftEncoder(), hal.getRightEncoder());
#else
#ifdef HUMAN_MODE
    HumanMessageDispatcherFactory factory;
#else
    ODROIDMessageDispatcherFactory factory;
#endif /*HUMAN_MODE*/

    MessageDispatcher& dispatcher = factory.getMessageDispatcher();

    PositionManager pm(hal.getLeftEncoder(), hal.getRightEncoder());

    MainFSMContext mainFSM(dispatcher, { }, { }, { &pm });

    // ////////////////////////////////////////////
    // Setup MessageHandlers
    // ////////////////////////////////////////////
    dispatcher.registerMessageHandler<ResetOdometryMessage>(
            [&pm](ResetOdometryMessage rom) {
                pm.reset(rom.getPosition(), rom.getHeading());
            });

    hal.getLeftMotor().enable();
    hal.getRightMotor().enable();

    dispatcher.registerMessageHandler<ControlledDriveMessage>(
            [&hal](const ControlledDriveMessage&) {
                hal.getLeftMotor().setSpeed(4000);
                hal.getRightMotor().setSpeed(4000);
            });
    dispatcher.registerMessageHandler<StopMessage>([&hal](const StopMessage&) {
        hal.getLeftMotor().stop();
        hal.getRightMotor().stop();
    });

    // ////////////////////////////////////////////
    // Setup Tasks
    // ////////////////////////////////////////////

    schedule_repeating_task([&mainFSM]() {
        mainFSM.tick();
    }, 10);

#ifdef BLINK_LED
    schedule_repeating_task([&hal]() {
        hal.getStatusLED().toggle();
    }, 250);
#endif

    // ////////////////////////////////////////////
    // BEGIN TEST AREA
    // ////////////////////////////////////////////

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
    hal.getServo().enable();
#endif
    schedule_repeating_task([&hal]() {
        hal.getLeftMotor().setSpeed(1000);
        hal.getRightMotor().setSpeed(1000);

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
            hal.getServo().moveTo(150_deg);
#endif
        }, 5000);
    schedule_repeating_task([&hal]() {
        hal.getLeftMotor().setSpeed(-1000);
        hal.getRightMotor().setSpeed(-1000);

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
            hal.getServo().moveTo(100_deg);
#endif
        }, 5000, 2500);

    dispatcher.registerMessageHandler<StopMessage>([&hal](StopMessage) {
        hal.disableAllActors();
    });

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
