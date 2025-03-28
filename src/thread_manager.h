#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadManager {
public:
    // Constructor: Initializes the thread pool with the specified number of threads
    explicit ThreadManager(size_t threadCount);

    // Destructor: Ensures all threads are joined and resources are cleaned up
    ~ThreadManager();

    // Adds a new task to the thread pool's task queue
    void enqueueTask(std::function<void()> task);

    // Explicitly stops the thread manager
    void stop();

private:
    // Vector to hold worker threads
    std::vector<std::thread> workers;

    // Task queue to hold tasks to be executed
    std::queue<std::function<void()>> tasks;

    // Synchronization primitives
    std::mutex queueMutex;
    std::condition_variable condition;

    // Flag to indicate if the thread pool is stopping
    bool isStopped;
};

#endif // THREAD_MANAGER_H