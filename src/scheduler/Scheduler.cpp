/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scheduler
 * @{
 ******************************************************************************
 */

#include <scheduler/Scheduler.h>

#include "stm32l4xx.h"

void HAL_SYSTICK_Callback(void) {
    Scheduler::getScheduler().tick();
}

Scheduler& Scheduler::getScheduler() {
    // Meyers Singleton
    static Scheduler instance;
    return instance;
}

int Scheduler::addTask(std::function<void(void)> task, int delay, int period) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].isAvailable()) {
            tasks[i] = {task, delay, period};
            return i;
        }
    }
    return -1;
}

void Scheduler::removeTask(int id) {
    if (id < 0 || id >= MAX_TASKS) {
        return;
    }
    tasks[id] = {};
}

void Scheduler::run() {
    while (1) {
        for (int i = 0; i < MAX_TASKS; i++) {
            tasks[i].execute();
        }
        // TODO: maybe sleep here?
    }
}

void Scheduler::tick() {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].tick();
    }
}

void start_scheduler() {
    Scheduler::getScheduler().run();
}

int schedule_task(std::function<void(void)> task, int delay) {
    return Scheduler::getScheduler().addTask(task, delay, 0);
}

int schedule_repeating_task(std::function<void(void)> task, int period,
        int delay) {
    return Scheduler::getScheduler().addTask(task, delay, period);
}

void unschedule_task(int id) {
    Scheduler::getScheduler().removeTask(id);
}

/** @} */
