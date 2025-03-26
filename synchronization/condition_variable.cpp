#include "condition_variable.h"

void ConditionVariable::wait(std::unique_lock<std::mutex>& lock) {
    std::unique_lock<std::mutex> internalLock(internalMutex, std::defer_lock);
    lock.unlock();
    internalLock.lock();
    lock.lock();
}

void ConditionVariable::notify_one() {
    std::lock_guard<std::mutex> lock(internalMutex);
    condition.notify_one();
}

void ConditionVariable::notify_all() {
    std::lock_guard<std::mutex> lock(internalMutex);
    condition.notify_all();
}