#include "util/Logger.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>

#include "spdlog/sinks/sink.h"


namespace util
{

std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;
Logger::LogDirection Logger::_logDirection = Logger::LogDirection::CONSOLE;
std::string Logger::_logFilePath = "../logs/log.txt";
bool Logger::_initialized = false;

void Logger::init()
{
    if (_initialized)
    {
        return;
    }

    updateSinks();
    _initialized = true;
}

std::shared_ptr<spdlog::logger> Logger::getLogger()
{
    if (!_initialized)
    {
        init();
    }
    return _logger;
}

void Logger::updateSinks()
{
    /**
     * Reconfigure the logger's sinks based on the current log direction and file path.
     * 1. Clear existing sinks.
     * 2. Add console sink if direction is CONSOLE or BOTH and same for file sink.
     * 3. If direction is NONE, create a null logger that does not log anything.
     * 4. Set appropriate log levels and patterns for each sink.
     * 5. Flush on all levels to ensure logs are written immediately.
     */

    std::vector<spdlog::sink_ptr> sinks;

    if (_logDirection == LogDirection::CONSOLE || _logDirection == LogDirection::BOTH)
    {
        // Create console sink with color support and pattern: {[timestamp] [level] message}
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
        sinks.push_back(console_sink);
    }

    if (_logDirection == LogDirection::FILE || _logDirection == LogDirection::BOTH)
    {
        // Ensure log directory exists before creating file sink
        size_t lastSlash = _logFilePath.find_last_of("/\\");
        if (lastSlash != std::string::npos)
        {
            std::string dirPath = _logFilePath.substr(0, lastSlash);
            std::filesystem::create_directories(dirPath);
        }

        // Create file sink with pattern: {[timestamp] [level] message}
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_logFilePath, true);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
        sinks.push_back(file_sink);
    }

    if (sinks.empty())
    {
        // If NONE, create a null logger that does not log anything
        _logger = std::make_shared<spdlog::logger>("grabx", spdlog::sinks_init_list{});
        _logger->set_level(spdlog::level::off);
    }
    else
    {
        // Create logger with the configured sinks (console and/or file)
        _logger = std::make_shared<spdlog::logger>("grabx", sinks.begin(), sinks.end());
        _logger->set_level(spdlog::level::trace);
    }

    // Flush on all levels to ensure logs are written immediately
    _logger->flush_on(spdlog::level::trace);
}

void Logger::info(const std::string& message) { getLogger()->info(message); }

void Logger::error(const std::string& message) { getLogger()->error(message); }

void Logger::debug(const std::string& message, const char* file, int line)
{
    if (file != nullptr && line > 0)
    {
        // Extract just the filename from the full path
        std::string filename = file;
        size_t lastSlash = filename.find_last_of("/\\");
        if (lastSlash != std::string::npos)
        {
            filename = filename.substr(lastSlash + 1);
        }
        getLogger()->debug("[{}:{}] {}", filename, line, message);
    }
    else
    {
        getLogger()->debug(message);
    }
}

void Logger::warn(const std::string& message) { getLogger()->warn(message); }

void Logger::fatal(const std::string& message) { getLogger()->critical(message); }

void Logger::setLogDirection(const LogDirection& direction)
{
    _logDirection = direction;
    if (_initialized)
    {
        updateSinks();
    }
}

void Logger::setLogFile(const std::string& filePath)
{
    _logFilePath = filePath;
    if (_initialized)
    {
        updateSinks();
    }
}

} // namespace util
