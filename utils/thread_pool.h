#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    // Constructor: Initializes the thread pool with the specified number of threads
    explicit ThreadPool(size_t threadCount);

    // Destructor: Ensures all threads are joined and resources are cleaned up
    ~ThreadPool();

    // Adds a new task to the thread pool's task queue
    void enqueueTask(std::function<void()> task);

private:
    // Worker threads
    std::vector<std::thread> workers;

    // Task queue
    std::queue<std::function<void()>> tasks;

    // Synchronization primitives
    std::mutex queueMutex;
    std::condition_variable condition;

    // Flag to indicate if the thread pool is stopping
    std::atomic<bool> stop;

    // Worker function for threads
    void workerThread();
};

#endif // THREAD_POOL_H