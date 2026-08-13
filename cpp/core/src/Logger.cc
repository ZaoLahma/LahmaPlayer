#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace LahmaPlayer
{

void Logger::init(const std::string &logPath, bool console)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_minLevel = LogLevel::INFO;
    m_consoleOutput = console;
    m_fileStream = nullptr;

    // Close any existing file stream
    if (m_fileStream)
    {
        m_fileStream->close();
    }

    // Open file if path is provided
    if (!logPath.empty())
    {
        try
        {
            m_fileStream = new std::ofstream(logPath, std::ios::app);
            if (!m_fileStream->is_open())
            {
                std::cerr << "Failed to open log file: " << logPath << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create log file: " << e.what() << std::endl;
        }
    }
}

void Logger::shutdown()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_fileStream)
    {
        m_fileStream->close();
        delete m_fileStream;
        m_fileStream = nullptr;
    }
}

void Logger::log(LogLevel level, const std::string &message, bool flush)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if level should be logged
    if (level < m_minLevel && m_minLevel != LogLevel::DEBUG)
    {
        return;
    }

    // Get timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream timestamp;
    timestamp << std::put_time(std::gmtime(&time_t), "%Y-%m-%d %H:%M:%S");
    timestamp << "." << std::setfill('0') << std::setw(3) << ms.count();

    std::ostringstream logLine;
    logLine << "[" << timestamp.str() << "] ";

    // Add level prefix
    switch (level)
    {
    case LogLevel::DEBUG: logLine << "[DEBUG] "; break;
    case LogLevel::INFO: logLine << "[INFO] "; break;
    case LogLevel::WARNING: logLine << "[WARNING] "; break;
    case LogLevel::ERROR: logLine << "[ERROR] "; break;
    }

    // Add message
    logLine << message;

    std::string logMessage = logLine.str();

    // Write to file if available
    if (m_fileStream)
    {
        *m_fileStream << logMessage << std::endl;
        if (flush)
        {
            m_fileStream->flush();
        }
    }

    // Write to console
    if (m_consoleOutput)
    {
        std::cout << logMessage << std::endl;
    }
}

void Logger::debug(const std::string &fmt)
{
    log(LogLevel::DEBUG, fmt, false);
}

void Logger::info(const std::string &fmt)
{
    log(LogLevel::INFO, fmt, false);
}

void Logger::warning(const std::string &fmt)
{
    log(LogLevel::WARNING, fmt, true);
}

void Logger::error(const std::string &fmt)
{
    log(LogLevel::ERROR, fmt, true);
}

} // namespace LahmaPlayer
