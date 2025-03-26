#include "thread_pool.h"
#include <stdexcept>

ThreadPool::ThreadPool(size_t threadCount) : stop(false) {
    if (threadCount == 0) {
        throw std::invalid_argument("Thread count must be greater than 0");
    }

    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        if (stop) {
            throw std::runtime_error("Cannot enqueue tasks on a stopped ThreadPool");
        }

        tasks.emplace(std::move(task));
    }

    condition.notify_one();
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() {
                return stop || !tasks.empty();
            });

            if (stop && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}