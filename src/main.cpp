#include <util/Logger.h>

int main()
{
    util::Logger::setLogDirection(util::Logger::LogDirection::BOTH);
    util::Logger::info("Application started");
    return 0;
}