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

/**
 * Override Systick-Callback from CubeHAL
 * @attention This is called from an ISR!
 */
void HAL_SYSTICK_Callback(void) {
    Scheduler::getScheduler().tick();
}

/**
 * Singleton implementation
 *
 * @return the Singleton instance
 */
Scheduler& Scheduler::getScheduler() {
    // Meyers Singleton
    static Scheduler instance;
    return instance;
}

/**
 * Add a task to be scheduled.
 *
 * @param task   the task to execute
 * @param delay  the delay to wait before executing
 * @param period the period (for repeating tasks)
 *
 * @return the id of the task for removal
 * @retval -1 unable to add task because all slots are full
 */
int Scheduler::addTask(std::function<void(void)> task, int delay, int period) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].isPlaceholder()) {
            tasks[i] = {task, delay, period};
            return i;
        }
    }
    return -1;
}

/**
 * Add a task to be scheduled from an ISR.
 *
 * @param task   the task to execute
 */
void Scheduler::addISRTask(std::function<void(void)> task) {
    // last task is reserved for ISR
    tasks[MAX_TASKS] = {task, 0 , 0};
}

/**
 * Remove a scheduled task
 *
 * @param id the id of the task to remove
 */
void Scheduler::removeTask(int id) {
    if (id < 0 || id >= MAX_TASKS) {
        return;
    }
    tasks[id] = {};
}

/**
 * Executes the dispatcher which in turn will run the tasks when they are due
 */
void Scheduler::run() {
    while (1) {
        for (int i = 0; i < TASK_COUNT; i++) {
            tasks[i].execute();
        }
        // TODO: maybe sleep here?
    }
}

/**
 * Tick all tasks.
 * @attention This is called from an ISR!
 */
void Scheduler::tick() {
    for (int i = 0; i < TASK_COUNT; i++) {
        tasks[i].tick();
    }
}

/**
 * Wrapper: run the dispatcher
 */
void start_scheduler() {
    Scheduler::getScheduler().run();
}

/**
 * Wrapper: schedule a one-shot task.
 *
 * @param task  the task to run
 * @param delay (optional) the delay to wait before executing
 *
 * @return the id of the task for removal
 * @retval -1 unable to add task because all slots are full
 */
int schedule_task(std::function<void(void)> task, int delay) {
    return Scheduler::getScheduler().addTask(task, delay, 0);
}

/**
 * Wrapper: schedule a repeating task.
 *
 * @param task   the task to run repeatedly
 * @param period the period of repetition
 * @param delay  (optional) initial delay to wait before first execution
 *
 * @return the id of the task for removal
 * @retval -1 unable to add task because all slots are full
 */
int schedule_repeating_task(std::function<void(void)> task, int period,
        int delay) {
    return Scheduler::getScheduler().addTask(task, delay, period);
}

/**
 * Wrapper: schedule a task from an ISR to defer it to normal context
 *
 * @param task   the task to be run in normal context
 */
void schedule_task_from_isr(std::function<void(void)> task) {
    Scheduler::getScheduler().addISRTask(task);
}

/**
 * Remove a task that is scheduled.
 *
 * @param id the is of the task to be removed
 */
void unschedule_task(int id) {
    Scheduler::getScheduler().removeTask(id);
}
/** @} */
