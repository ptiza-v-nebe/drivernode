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
    bool placeholder; ///< is the task a placeholder or actually an executable task
    std::function<void(void)> function; ///< the task to execute
    volatile int delay; ///< delay to wait until next execution
    int period; ///< repetition period (0 for one-shot)
    volatile int execCounter; ///< how often is the task due to be executed
public:
    Task();
    Task(std::function<void(void)> function, int delay, int period);

    bool isPlaceholder();

    void execute();
    void tick();

private:
    bool canExecute();
    void reset();
};

#endif /* SCHEDULER_TASK_H_ */
/** @} */
