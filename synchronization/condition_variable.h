#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

#include <mutex>
#include <condition_variable>

class ConditionVariable {
public:
    // Waits on the condition variable
    void wait(std::unique_lock<std::mutex>& lock);

    // Notifies one waiting thread
    void notify_one();

    // Notifies all waiting threads
    void notify_all();

private:
    // Internal mutex for synchronization
    std::mutex internalMutex;

    // Standard condition variable
    std::condition_variable condition;
};

#endif // CONDITION_VARIABLE_H