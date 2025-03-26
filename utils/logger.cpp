#include "logger.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filePath) {
    logFile.open(filePath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto localTime = std::localtime(&time_t_now);

    // Write timestamp and message to the log file
    logFile << "[" << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;

    if (!logFile) {
        throw std::runtime_error("Failed to write to log file");
    }
}