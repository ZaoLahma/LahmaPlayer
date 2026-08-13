#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>

namespace LahmaPlayer {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void init(const std::string& logPath = "", bool console = true);
    void shutdown();

    // Convenience methods with format strings
    void debug(const std::string& fmt);
    void info(const std::string& fmt);
    void warning(const std::string& fmt);
    void error(const std::string& fmt);

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(LogLevel level, const std::string& message, bool flush);

    std::ofstream* m_fileStream = nullptr;
    std::mutex m_mutex;
    LogLevel m_minLevel = LogLevel::INFO;
    bool m_consoleOutput = true;
};

} // namespace LahmaPlayer
