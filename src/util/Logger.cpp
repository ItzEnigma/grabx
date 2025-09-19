#include "util/Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <filesystem>

namespace util
{
    Logger::LogDirection Logger::logDirection = Logger::LogDirection::CONSOLE;
    std::string Logger::logFilePath = "../logs/log.txt";

    std::mutex Logger::logMutex;
    std::ofstream Logger::logFile;
    bool Logger::isFileOpen = false;

    /**
     * The `log` function logs a message with a specified level to either the console, a file, or both,
     * along with the current time.
     *
     * @param level The `level` parameter in the `log` function represents the log level of the message
     * being logged. It is a string that indicates the severity or importance of the log message, such as
     * "INFO", "WARNING", "ERROR", etc.
     * @param message The `message` parameter in the `log` function represents the actual log message that
     * you want to log. It is a `std::string` type and contains the information or content that you want to
     * log, such as an event description, error message, or any other relevant information that needs to
     *
     * @return If the log file fails to open, the message "Failed to open log file!" will be printed to the
     * standard error stream (std::cerr). No value is explicitly returned from the `log` function in this
     * case.
     */
    void Logger::log(const std::string &level, const std::string &message)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logDirection == LogDirection::NONE) // If log direction is NONE, do not log anything
        {
            return;
        }
        std::ostringstream oss;
        oss << " [" << level << "] " << getCurrentTime() << " - " << message;
        std::string logMessage = oss.str();

        /**
         * Check if the log direction is set to console or both, and log to console.
         */
        if (logDirection == LogDirection::CONSOLE || logDirection == LogDirection::BOTH)
        {
            std::cout << logMessage << std::endl;
        }
        if (logDirection == LogDirection::FILE || logDirection == LogDirection::BOTH)
        {
            if (!isFileOpen)
            {
                // Ensure the log directory exists before opening the file
                size_t lastSlash = logFilePath.find_last_of("/\\");
                if (lastSlash != std::string::npos)
                {
                    std::string dirPath = logFilePath.substr(0, lastSlash);
                    // Try to create the directory if it doesn't exist
                    std::filesystem::create_directories(dirPath);
                }
                logFile.open(logFilePath, std::ios::app);
                if (!logFile.is_open())
                {
                    std::cerr << "Failed to open log file!" << std::endl;
                    return;
                }
                isFileOpen = true;
            }
            logFile << logMessage << std::endl;
            logFile.flush();
        }
    }

    /**
     * The `getCurrentTime` function in C++ returns the current time in the format "%Y-%m-%d %H:%M:%S".
     *
     * @return The `getCurrentTime` function returns a `std::string` containing the current date and time
     * in the format "YYYY-MM-DD HH:MM:SS".
     */
    std::string Logger::getCurrentTime()
    {
        std::time_t now = std::time(nullptr);
        std::tm *tm_now = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void Logger::setLogDirection(const LogDirection &direction)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        logDirection = direction;
    }

    void Logger::setLogFile(const std::string &filePath)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        if (isFileOpen)
        {
            logFile.close();
            isFileOpen = false;
        }
        logFilePath = filePath;
    }

    void Logger::info(const std::string &message)
    {
        log("INFO", message);
    }

    void Logger::error(const std::string &message)
    {
        log("ERROR", message);
    }

    void Logger::debug(const std::string &message)
    {
        log("DEBUG", message);
    }

    void Logger::warn(const std::string &message)
    {
        log("WARN", message);
    }

    void Logger::fatal(const std::string &message)
    {
        log("FATAL", message);
    }
}