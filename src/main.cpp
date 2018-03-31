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

    TIM_HandleTypeDef timer = {0};
    TIM_OC_InitTypeDef channel = {0};
    GPIO_InitTypeDef gpio = getDefaultGPIO();

    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    timer.Init.Prescaler = 80 - 1;
    timer.Init.Period = 1000 - 1;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Instance = TIM2;

    channel.Pulse = 500;
    channel.OCMode = TIM_OCMODE_PWM1;

    gpio.Alternate = GPIO_AF1_TIM2;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_0;

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_TIM_PWM_Init(&timer);
    HAL_TIM_PWM_ConfigChannel(&timer, &channel, TIM_CHANNEL_1);
    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1);

    PWM pwm(TIM2, TIM_CHANNEL_1);
    pwm.setFrequency(1);
    pwm.setDutyCycle(0.25);

    dispatcher.registerMessageHandler<SetSpeedMessage>([&pwm](SetSpeedMessage ssm){
       if(ssm.getSpeedLeft()) {
           pwm.enable();

       } else {
           pwm.disable();
       }
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
