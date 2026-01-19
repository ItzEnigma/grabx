#include <util/Logger.h>

#include "db/model/Download.h"

int main()
{
    util::Logger::setLogDirection(util::Logger::LogDirection::BOTH);
    util::Logger::info("Application started");

    // Example of debug logging with file and line information
    LOG_DEBUG("Debug message with file and line info");

    // Traditional method still works
    util::Logger::debug("Debug message without line info");

    // New format-style logging with spdlog
    int x = 20;
    int y = 50;
    util::Logger::info("x = {}, y = {}", x, y);
    util::Logger::debug("Values: x = {}, y = {}", x, y);
    util::Logger::warn("Warning: x = {}, y = {}", x, y);
    util::Logger::error("Error values: x = {}, y = {}", x, y);

    // More complex formatting examples
    util::Logger::info("Hexadecimal: x = {:#x}, y = {:#x}", x, y);
    util::Logger::info("Float precision: pi = {:.2f}", 3.14159);
    util::Logger::info("Multiple types: int = {}, float = {:.3f}, string = {}", 42, 3.14159, "hello");
    util::Logger::fatal("This fatal message will also not be logged");

    util::Logger::setLogDirection(util::Logger::LogDirection::NONE);
    util::Logger::info("This message will not be logged");

    LOG_DEBUG("This debug message will also not be logged");

    return 0;
}
