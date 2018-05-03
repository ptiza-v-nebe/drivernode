/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scheduler
 * @{
 ******************************************************************************
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include "scheduler/Task.h"
#include <functional>

/**
 * Maximum number of task that can be scheduled at a time
 */
constexpr int MAX_TASKS = 10;

/**
 * Number of slots to allocate.
 * Maximum number of tasks + extra task for scheduling from within ISRs
 */
constexpr int TASK_COUNT = MAX_TASKS + 1;

class Scheduler {
public:
    static Scheduler& getScheduler(); //Singleton
private:
    Task tasks[TASK_COUNT]; ///< buffer for scheduling slots
private:
    Scheduler() = default;
public:
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete;

    int addTask(std::function<void(void)> task, int delay, int period);
    void addISRTask(std::function<void(void)> task);
    void removeTask(int id);
    void run();
    void tick();
};

void start_scheduler();

int schedule_task(std::function<void(void)> task, int delay = 0);
int schedule_repeating_task(std::function<void(void)> task, int period, int delay = 0);
void unschedule_task(int id);
void schedule_task_from_isr(std::function<void(void)> task);

#endif /* SCHEDULER_SCHEDULER_H_ */
/** @} */
