/*
THEORY: Logger (Singleton Pattern)
- Thread-safe logging with multiple severity levels
- Uses Meyer's Singleton for thread-safe lazy initialization
- Supports multiple sinks (console, file)
- Each log entry has timestamp, severity, and message
- Fluent interface for easy usage

Complexity: O(1) per log entry
Interview Questions:
1. Why Meyer's Singleton is thread-safe? (static local initialization guaranteed by C++11)
2. How would you make logger asynchronous? (queue + worker thread)
3. How to handle log rotation? (size-based file rotation)
Common Mistakes:
- Forgetting to synchronize output operations
- Creating singleton before main() causing static initialization order fiasco
- Not using move semantics for string parameters
*/

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>

enum class LogLevel {
    DEBUG, INFO, WARNING, ERROR, FATAL
};

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    Logger& setLogFile(const std::string& filename) {
        std::lock_guard lock(mutex_);
        fileSink_.open(filename, std::ios::app);
        return *this;
    }

    Logger& setMinLevel(LogLevel level) {
        std::lock_guard lock(mutex_);
        minLevel_ = level;
        return *this;
    }

    Logger& log(LogLevel level, const std::string& message) {
        std::lock_guard lock(mutex_);
        if (level < minLevel_) return *this;
        auto entry = formatEntry(level, message);
        consoleSink(entry);
        if (fileSink_.is_open()) {
            fileSink_ << entry << std::flush;
        }
        return *this;
    }

    Logger& debug(const std::string& msg) { return log(LogLevel::DEBUG, msg); }
    Logger& info(const std::string& msg) { return log(LogLevel::INFO, msg); }
    Logger& warning(const std::string& msg) { return log(LogLevel::WARNING, msg); }
    Logger& error(const std::string& msg) { return log(LogLevel::ERROR, msg); }
    Logger& fatal(const std::string& msg) { return log(LogLevel::FATAL, msg); }

private:
    Logger() : minLevel_(LogLevel::DEBUG) {}
    ~Logger() { if (fileSink_.is_open()) fileSink_.close(); }
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string levelToString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            case LogLevel::FATAL:   return "FATAL";
            default:                return "UNKNOWN";
        }
    }

    std::string formatEntry(LogLevel level, const std::string& message) const {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << ms.count()
            << " [" << levelToString(level) << "] " << message;
        return oss.str();
    }

    void consoleSink(const std::string& entry) const {
        std::cout << entry << std::endl;
    }

    std::mutex mutex_;
    LogLevel minLevel_;
    std::ofstream fileSink_;
};

/*
SAMPLE OUTPUT:
2026-06-12 10:30:45.123 [INFO] Application started
2026-06-12 10:30:45.124 [DEBUG] Initializing modules...
2026-06-12 10:30:45.125 [ERROR] Failed to load config file
*/

int main() {
    Logger::instance()
        .setLogFile("app.log")
        .setMinLevel(LogLevel::DEBUG)
        .info("Application started")
        .debug("Initializing modules...")
        .warning("Low memory")
        .error("Failed to load config file");

    return 0;
}
