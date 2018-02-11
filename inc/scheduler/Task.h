/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scheduler
 * @{
 ******************************************************************************
 */

#ifndef SCHEDULER_TASK_H_
#define SCHEDULER_TASK_H_

#include <functional>

class Task {
private:
    bool valid;
    std::function<void(void)> function;
    volatile int delay;
    int period;
    volatile int execCounter;
public:
    Task();
    Task(std::function<void(void)> function, int delay, int period);

    bool isAvailable();
    bool canExecute();

    void execute();
    void tick();
};

#endif /* SCHEDULER_TASK_H_ */
/** @} */
