#include "../synchronization/mutex.h"
#include "../synchronization/condition_variable.h"
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <atomic>
#include <chrono>

void testMutex() {
    Mutex mutex;
    int sharedCounter = 0;

    auto incrementTask = [&]() {
        for (int i = 0; i < 1000; ++i) {
            mutex.lock();
            ++sharedCounter;
            mutex.unlock();
        }
    };

    std::thread t1(incrementTask);
    std::thread t2(incrementTask);

    t1.join();
    t2.join();

    // Verify that the shared counter was incremented correctly
    assert(sharedCounter == 2000);
    std::cout << "Test passed: Mutex synchronization\n";
}

void testConditionVariable() {
    ConditionVariable conditionVariable;
    Mutex mutex;
    bool ready = false;
    int result = 0;

    auto producer = [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mutex.lock();
        ready = true;
        result = 42;
        mutex.unlock();
        conditionVariable.notify_one();
    };

    auto consumer = [&]() {
        std::unique_lock<std::mutex> lock(mutex.getInternalMutex());
        while (!ready) {
            conditionVariable.wait(lock);
        }
        assert(result == 42);
    };

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    std::cout << "Test passed: ConditionVariable synchronization\n";
}

int main() {
    testMutex();
    testConditionVariable();

    std::cout << "All synchronization tests passed!\n";
    return 0;
}