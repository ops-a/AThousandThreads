#include "../src/thread_manager.h"
#include <iostream>
#include <cassert>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <vector>
#include <iomanip>
#include <cmath>

// Function to simulate a CPU-intensive task
void cpuIntensiveTask(int iterations) {
    volatile double result = 0.0;
    for (int i = 0; i < iterations; ++i) {
        result += std::sin(i) * std::cos(i);
    }
}

// Function to simulate an I/O-bound task
void ioIntensiveTask(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Sequential execution of tasks
void runSequential(const std::vector<std::function<void()>>& tasks) {
    for (const auto& task : tasks) {
        task();
    }
}

// Parallel execution using ThreadManager
void runParallel(ThreadManager& threadManager, const std::vector<std::function<void()>>& tasks) {
    for (const auto& task : tasks) {
        threadManager.enqueueTask(task);
    }
}

// Test CPU-intensive tasks
void testCPUIntensiveTasks(size_t threadCount, int taskCount, int iterationsPerTask) {
    std::cout << "\n===== CPU-Intensive Tasks Test =====" << std::endl;
    std::cout << "Thread count: " << threadCount << ", Task count: " << taskCount << std::endl;
    
    // Create tasks
    std::vector<std::function<void()>> tasks;
    for (int i = 0; i < taskCount; ++i) {
        tasks.push_back([iterationsPerTask]() {
            cpuIntensiveTask(iterationsPerTask);
        });
    }
    
    // Sequential execution
    auto startSeq = std::chrono::high_resolution_clock::now();
    runSequential(tasks);
    auto endSeq = std::chrono::high_resolution_clock::now();
    auto durationSeq = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();
    
    std::cout << "Sequential execution time: " << durationSeq << " ms" << std::endl;
    
    // Parallel execution
    ThreadManager threadManager(threadCount);
    auto startPar = std::chrono::high_resolution_clock::now();
    runParallel(threadManager, tasks);
    
    // Wait for all tasks to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    threadManager.stop();
    
    auto endPar = std::chrono::high_resolution_clock::now();
    auto durationPar = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();
    
    std::cout << "Parallel execution time: " << durationPar << " ms" << std::endl;
    std::cout << "Speedup factor: " << std::fixed << std::setprecision(2) 
              << static_cast<double>(durationSeq) / durationPar << "x" << std::endl;
}

// Test I/O-intensive tasks
void testIOIntensiveTasks(size_t threadCount, int taskCount, int millisecondsPerTask) {
    std::cout << "\n===== I/O-Intensive Tasks Test =====" << std::endl;
    std::cout << "Thread count: " << threadCount << ", Task count: " << taskCount << std::endl;
    
    // Create tasks
    std::vector<std::function<void()>> tasks;
    for (int i = 0; i < taskCount; ++i) {
        tasks.push_back([millisecondsPerTask]() {
            ioIntensiveTask(millisecondsPerTask);
        });
    }
    
    // Sequential execution
    auto startSeq = std::chrono::high_resolution_clock::now();
    runSequential(tasks);
    auto endSeq = std::chrono::high_resolution_clock::now();
    auto durationSeq = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();
    
    std::cout << "Sequential execution time: " << durationSeq << " ms" << std::endl;
    
    // Parallel execution
    ThreadManager threadManager(threadCount);
    auto startPar = std::chrono::high_resolution_clock::now();
    runParallel(threadManager, tasks);
    
    // Wait for all tasks to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(taskCount * millisecondsPerTask / threadCount + 100));
    threadManager.stop();
    
    auto endPar = std::chrono::high_resolution_clock::now();
    auto durationPar = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();
    
    std::cout << "Parallel execution time: " << durationPar << " ms" << std::endl;
    std::cout << "Speedup factor: " << std::fixed << std::setprecision(2) 
              << static_cast<double>(durationSeq) / durationPar << "x" << std::endl;
}

// Test mixed tasks (both CPU and I/O intensive)
void testMixedTasks(size_t threadCount, int taskCount) {
    std::cout << "\n===== Mixed Tasks Test =====" << std::endl;
    std::cout << "Thread count: " << threadCount << ", Task count: " << taskCount << std::endl;
    
    // Create tasks
    std::vector<std::function<void()>> tasks;
    for (int i = 0; i < taskCount; ++i) {
        if (i % 2 == 0) {
            // CPU-intensive task
            tasks.push_back([]() {
                cpuIntensiveTask(1000000);
            });
        } else {
            // I/O-intensive task
            tasks.push_back([]() {
                ioIntensiveTask(50);
            });
        }
    }
    
    // Sequential execution
    auto startSeq = std::chrono::high_resolution_clock::now();
    runSequential(tasks);
    auto endSeq = std::chrono::high_resolution_clock::now();
    auto durationSeq = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();
    
    std::cout << "Sequential execution time: " << durationSeq << " ms" << std::endl;
    
    // Parallel execution
    ThreadManager threadManager(threadCount);
    auto startPar = std::chrono::high_resolution_clock::now();
    runParallel(threadManager, tasks);
    
    // Wait for all tasks to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    threadManager.stop();
    
    auto endPar = std::chrono::high_resolution_clock::now();
    auto durationPar = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();
    
    std::cout << "Parallel execution time: " << durationPar << " ms" << std::endl;
    std::cout << "Speedup factor: " << std::fixed << std::setprecision(2) 
              << static_cast<double>(durationSeq) / durationPar << "x" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_threads>" << std::endl;
        return 1;
    }

    size_t threadCount = std::atoi(argv[1]);
    if (threadCount == 0) {
        std::cerr << "Error: Number of threads must be greater than 0" << std::endl;
        return 1;
    }

    std::cout << "Running performance comparison tests with " << threadCount << " threads" << std::endl;
    
    // Run CPU-intensive tests
    testCPUIntensiveTasks(threadCount, 10, 1000000);
    
    // Run I/O-intensive tests
    testIOIntensiveTasks(threadCount, 10, 100);
    
    // Run mixed tests
    testMixedTasks(threadCount, 20);
    
    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}