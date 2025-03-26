#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    // Constructor: Initializes the logger with a file path
    explicit Logger(const std::string& filePath);

    // Destructor: Ensures the log file is closed
    ~Logger();

    // Logs a message to the file
    void log(const std::string& message);

private:
    // File stream for logging
    std::ofstream logFile;

    // Mutex for thread-safe logging
    std::mutex logMutex;
};

#endif // LOGGER_H