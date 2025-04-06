// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>  // Add this line


enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
private:
    std::string logFilename;
    LogLevel minLevel;
    std::ofstream logFile;
    static std::mutex logMutex;  // For thread safety
    
    // Private constructor for singleton pattern
    Logger(const std::string& filename, LogLevel level = LogLevel::INFO)
        : logFilename(filename), minLevel(level) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }
    
    // Get current timestamp as string
    std::string getCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    // Convert LogLevel to string
    std::string logLevelToString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG:    return "DEBUG";
            case LogLevel::INFO:     return "INFO";
            case LogLevel::WARNING:  return "WARNING";
            case LogLevel::ERROR:    return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default:                 return "UNKNOWN";
        }
    }
    
public:
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Singleton instance accessor
    static Logger& getInstance() {
        static Logger instance("logs/crm.log");
        return instance;
    }
    
    // Destructor
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    // Set minimum log level
    void setMinLevel(LogLevel level) {
        minLevel = level;
    }
    
    // Log a message
    void log(LogLevel level, const std::string& message) {
        if (level < minLevel) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(logMutex);
        
        if (logFile.is_open()) {
            logFile << getCurrentTimestamp() << " [" << logLevelToString(level) << "] " 
                   << message << std::endl;
            logFile.flush();
        }
        
        // Also print to console for higher levels
        if (level >= LogLevel::WARNING) {
            std::cout << "[" << logLevelToString(level) << "] " << message << std::endl;
        }
    }
    
    // Convenience methods for different log levels
    void debug(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }
    
    void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }
    
    void warning(const std::string& message) {
        log(LogLevel::WARNING, message);
    }
    
    void error(const std::string& message) {
        log(LogLevel::ERROR, message);
    }
    
    void critical(const std::string& message) {
        log(LogLevel::CRITICAL, message);
    }
};

// Initialize static mutex
// Add 'extern' keyword:
extern std::mutex logMutex;
#endif // LOGGER_H