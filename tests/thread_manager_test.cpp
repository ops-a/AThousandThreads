#include "../src/thread_manager.h"
#include <iostream>
#include <cassert>
#include <atomic>
#include <chrono>
#include <cstdlib>

void testThreadManagerBasicFunctionality(size_t threadCount) {
    std::atomic<int> counter = 0;
    ThreadManager threadManager(threadCount);

    // Enqueue 10 tasks to increment the counter
    for (int i = 0; i < 10; ++i) {
        threadManager.enqueueTask([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    // Allow some time for tasks to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Verify that all tasks were executed
    assert(counter == 10);
    std::cout << "Test passed: Basic functionality with " << threadCount << " threads\n";
}

void testThreadManagerExceptionOnStopped(size_t threadCount) {
    ThreadManager threadManager(threadCount);

    // Stop the thread manager by destroying it
    threadManager.~ThreadManager();

    try {
        threadManager.enqueueTask([]() {
            std::cout << "This task should not run\n";
        });
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        std::cout << "Test passed: Exception on stopped ThreadManager with " << threadCount << " threads\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_threads>\n";
        return 1;
    }

    size_t threadCount = std::atoi(argv[1]);
    if (threadCount == 0) {
        std::cerr << "Error: Number of threads must be greater than 0\n";
        return 1;
    }

    testThreadManagerBasicFunctionality(threadCount);
    testThreadManagerExceptionOnStopped(threadCount);

    std::cout << "All tests passed!\n";
    return 0;
}