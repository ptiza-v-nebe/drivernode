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

constexpr int MAX_TASKS = 10;

class Scheduler {
public:
    static Scheduler& getScheduler(); //Singleton
private:
    Task tasks[MAX_TASKS];
private:
    Scheduler() = default;
public:
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete;

    int addTask(std::function<void(void)> task, int delay, int period);
    void removeTask(int id);
    void run();
    void tick();
};

void start_scheduler();

int schedule_task(std::function<void(void)> task, int delay = 0);
int schedule_repeating_task(std::function<void(void)> task, int period, int delay = 0);
void unschedule_task(int id);

#endif /* SCHEDULER_SCHEDULER_H_ */
/** @} */
