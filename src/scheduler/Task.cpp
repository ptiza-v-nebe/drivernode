/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scheduler
 * @{
 ******************************************************************************
 */

#include "scheduler/Task.h"

Task::Task() :
        valid(false), delay(0), period(0), execCounter(0) {
}

Task::Task(std::function<void(void)> function, int delay, int period) :
        valid(true), function(function), delay(delay), period(period), execCounter(0) {
}

bool Task::isAvailable() {
    return !valid;
}

bool Task::canExecute() {
    return valid && execCounter > 0;
}

void Task::execute() {
    if(canExecute()){
        function();
        execCounter--;
        if(period == 0) {
            // mark slot as available
            valid = false;
        }
    }
}

void Task::tick() {
    if(valid){
        delay--;
        if(delay < 0){
            execCounter++;

            if(period != 0){
                delay = period;
            }
        }
    }
}
/** @} */
