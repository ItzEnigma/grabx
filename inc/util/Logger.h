#ifndef _LOGGER_H
#define _LOGGER_H

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace util
{
class Logger
{
public:
    enum class LogDirection
    {
        NONE,
        CONSOLE,
        FILE,
        BOTH
    };

    /**
     * Initialize the logger with default settings
     */
    static void init();

    /**
     * The `info` function logs an informational message.
     */
    static void info(const std::string& message);

    /**
     * The `info` function logs an informational message with format arguments.
     * @example Logger::info("x = {}, y = {}", 20, 50);
     */
    template <typename... Args> static void info(fmt::format_string<Args...> fmt, Args&&... args)
    {
        getLogger()->info(fmt, std::forward<Args>(args)...);
    }

    /**
     * The error function logs an error message.
     */
    static void error(const std::string& message);

    /**
     * The error function logs an error message with format arguments.
     * @example Logger::error("Could not open file: {}", filename);
     */
    template <typename... Args> static void error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        getLogger()->error(fmt, std::forward<Args>(args)...);
    }

    /**
     * The debug function logs a debug message with file and line information.
     * @note Use LOG_DEBUG macro for automatic file/line capture.
     */
    static void debug(const std::string& message, const char* file = nullptr, int line = 0);

    /**
     * The debug function logs a debug message with format arguments.
     * @example Logger::debug("Debug values: x = {}, y = {}", 20, 50);
     */
    template <typename... Args> static void debug(fmt::format_string<Args...> fmt, Args&&... args)
    {
        getLogger()->debug(fmt, std::forward<Args>(args)...);
    }

    /**
     * The warn function logs a warning message.
     */
    static void warn(const std::string& message);

    /**
     * The warn function logs a warning message with format arguments.
     * @example Logger::warn("Potential issue at x = {}, y = {}", 20, 50);
     */
    template <typename... Args> static void warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        getLogger()->warn(fmt, std::forward<Args>(args)...);
    }

    /**
     * The fatal function logs a critical/fatal message.
     */
    static void fatal(const std::string& message);

    /**
     * The fatal function logs a critical/fatal message with format arguments.
     * @example Logger::fatal("Critical error: variable buffer size = {}", size);
     */
    template <typename... Args> static void fatal(fmt::format_string<Args...> fmt, Args&&... args)
    {
        getLogger()->critical(fmt, std::forward<Args>(args)...);
    }

    /**
     * Set the log direction (console, file, or both).
     */
    static void setLogDirection(const LogDirection& direction);

    /**
     * Set the log file path.
     */
    static void setLogFile(const std::string& filePath);

private:
    static std::shared_ptr<spdlog::logger> getLogger();
    static void updateSinks(); // Reconfigure sinks based on current settings

    static std::shared_ptr<spdlog::logger> _logger; // The spdlog logger instance
    static LogDirection _logDirection;
    static std::string _logFilePath;
    static bool _initialized;
};
} // namespace util

// Macro for debug logging with file and line information
#define LOG_DEBUG(message) util::Logger::debug(message, __FILE__, __LINE__)

// Convenience macros for other log levels
#define LOG_INFO(message) util::Logger::info(message)
#define LOG_WARN(message) util::Logger::warn(message)
#define LOG_ERROR(message) util::Logger::error(message)
#define LOG_FATAL(message) util::Logger::fatal(message)

#endif // _LOGGER_H
