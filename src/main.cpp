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
    /*schedule_repeating_task([&pm]() {
     pm.update();
     }, 5);*/
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
                printf("Scara: Encoder %ld, EndSwitch: %s\r\n",
                        hal.getScaraHardware().getLiftEncoder().getTick(),
                        hal.getScaraHardware().getEndSwitch().isOn() ? "PRESSED" : "RELEASED");
#endif
                hal.getSRF08s()[0].startRanging();
                hal.getSRF08s()[1].startRanging();
            }, 500);
#endif

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
