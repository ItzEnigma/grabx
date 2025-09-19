#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

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
		 * The `info` function in the `Logger` class logs an informational message with the tag "INFO".
		 *
		 * @param message The `message` parameter is a constant reference to a `std::string` object, which
		 * means it is a read-only reference to a string that is passed to the `info` method of the `Logger`
		 * class.
		 */
		static void info(const std::string &message);

		/**
		 * The error function in the Logger class logs an error message with the tag "ERROR".
		 *
		 * @param message The `message` parameter is a constant reference to a `std::string` object, which
		 * contains the error message that needs to be logged.
		 */
		static void error(const std::string &message);

		/**
		 * The debug function in the Logger class logs a message with the "DEBUG" level.
		 *
		 * @param message The `message` parameter is a constant reference to a `std::string` object, which
		 * means it is a read-only reference to a string that is passed to the `debug` function.
		 */
		static void debug(const std::string &message);

		/**
		 * The `warn` function in the `Logger` class logs a warning message with the prefix "WARN".
		 *
		 * @param message The `message` parameter is a constant reference to a `std::string` object, which
		 * means it is a read-only reference to a string that is passed to the `warn` function in the `Logger`
		 * class.
		 */
		static void warn(const std::string &message);

		/**
		 * The `Logger::fatal` function logs a message with the severity level "FATAL".
		 *
		 * @param message The `message` parameter is a constant reference to a `std::string` object. It is the
		 * message that will be logged with the severity level "FATAL".
		 */
		static void fatal(const std::string &message);

		/**
		 * The function `setLogDirection` in the Logger class sets the log direction to the specified value.
		 *
		 * @param direction The `direction` parameter is of type `LogDirection`, which is a user-defined type
		 * that likely represents the direction in which logging should occur (e.g., file, console, network).
		 */
		static void setLogDirection(const LogDirection &direction);

		/**
		 * The `setLogFile` function in the `Logger` class sets the log file path.
		 * @param filePath The `filePath` parameter is a constant reference to a `std::string` object. It represents
		 * the path of the log file that you want to set for the logger.
		 */
		static void setLogFile(const std::string &filePath);

	private:
		static void log(const std::string &level, const std::string &message);
		static std::string getCurrentTime();
		static std::mutex logMutex;
		static std::ofstream logFile;
		static bool isFileOpen;
		static LogDirection logDirection;
		static std::string logFilePath;
	};
}

#endif // _LOGGER_H