#include "mutex.h"

void Mutex::lock() {
    internalMutex.lock();
}

void Mutex::unlock() {
    internalMutex.unlock();
}

bool Mutex::try_lock() {
    return internalMutex.try_lock();
}

std::mutex& Mutex::getInternalMutex() {
    return internalMutex;
}