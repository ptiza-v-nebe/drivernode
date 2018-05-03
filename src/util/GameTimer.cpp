/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */

#include <util/GameTimer.h>
#include <stm32l4xx.h>
#include <error.h>
#include <control/MainFSM.h>
#include <hal/HALManager.h>
#include <hal/interupts.h>

#include <climits>

static constexpr int GAME_TIME_s = 100;

extern "C" {
void TIM1_UP_TIM16_IRQHandler() {
    static GameTimer& gameTimer = GameTimer::getInstance();
    if (TIM16->SR & TIM_SR_UIF) {
        TIM16->SR &= ~TIM_SR_UIF;
        gameTimer.tick();
    }
}
}

GameTimer& GameTimer::getInstance() {
    static GameTimer instance;
    return instance;
}

GameTimer::GameTimer() :
        seconds(INT_MIN), fsm(nullptr) {
    initialize();
}

void GameTimer::initialize() {
    TIM_HandleTypeDef tim16 { };
    tim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    tim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim16.Init.Prescaler = 8000;
    tim16.Init.Period = 10000;

    tim16.Instance = TIM16;

    __HAL_RCC_TIM16_CLK_ENABLE()
    ;

    TRY(HAL_TIM_Base_Init(&tim16));
    TRY(HAL_TIM_Base_Start_IT(&tim16));
}

void GameTimer::tick() {
    seconds++;
    //printf("Game time is %d\r\n", seconds);

    if (seconds >= GAME_TIME_s) {
        if (fsm) {
            fsm->gameTimerFinished();
        }
        HALManager::getInstance().disableAllActors();
    }
}

void GameTimer::start() {
    seconds = 0;
    TIM16->CNT = 0;
    TIM16->SR &= ~TIM_SR_UIF;

    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, GAME_TIMER_PREEMPTION_PRIORITY,
            GAME_TIMER_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

void GameTimer::setFSM(MainFSM* fsm) {
    this->fsm = fsm;
}
/** @} */
