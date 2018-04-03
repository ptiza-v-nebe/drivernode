/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scheduler
 * @{
 ******************************************************************************
 */

#include "scheduler/Task.h"

/**
 * Constructs a placeholder Task.
 */
Task::Task() :
        Task([]() {}, 0, 0) {
}

/**
 * Constructs a Task.
 *
 * @param function the task to execute
 * @param delay    the initial delay to wait before first execution
 * @param period   the repetition period (0 if one-shot task)
 */
Task::Task(std::function<void(void)> function, int delay, int period) :
        placeholder(false), function(function), delay(delay), period(period), execCounter(
                0) {
    if (delay == 0) {
        reset();
    }
}

/**
 * @return Is the Task a placeholder or an actual executable task.
 */
bool Task::isPlaceholder() {
    return placeholder;
}

/**
 * @return should this task be executed
 */
bool Task::canExecute() {
    return !placeholder && execCounter > 0;
}

/**
 * Execute the Task (if it should be executed) or do nothing
 */
void Task::execute() {
    if (canExecute()) {
        function();
        execCounter--;
        if (period == 0) {
            // mark slot as placeholder
            placeholder = true;
        }
    }
}

/**
 * Tick the task.
 * Will decrease the delay and reset the task if the delay reaches 0
 * @attention This is called from an ISR!
 */
void Task::tick() {
    if (!placeholder) {
        delay--;
        if (delay == 0) {
            reset();
        }
    }
}

/**
 * Resets the task.
 * This will increase the execution counter and reset the delay if the task is periodic
 */
void Task::reset() {
    execCounter++;

    if (period != 0) {
        delay = period;
    }
}
/** @} */
