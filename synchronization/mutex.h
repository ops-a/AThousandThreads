#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>

class Mutex {
public:
    // Locks the mutex
    void lock();

    // Unlocks the mutex
    void unlock();

    // Tries to lock the mutex, returns true if successful
    bool try_lock();

    // Returns a reference to the internal mutex
    std::mutex& getInternalMutex();

private:
    // Internal standard mutex
    std::mutex internalMutex;
};

#endif // MUTEX_H