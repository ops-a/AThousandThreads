#include "thread_manager.h"
#include <iostream>
#include <stdexcept>

ThreadManager::ThreadManager(size_t threadCount) : stop(false) {
    if (threadCount == 0) {
        throw std::invalid_argument("Thread count must be greater than 0");
    }

    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty();
                    });

                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadManager::~ThreadManager() {
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

void ThreadManager::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        if (stop) {
            throw std::runtime_error("Cannot enqueue tasks on a stopped ThreadManager");
        }

        tasks.emplace(std::move(task));
    }

    condition.notify_one();
}