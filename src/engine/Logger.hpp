#ifndef DEBUG_LOGGER_HPP
#define DEBUG_LOGGER_HPP

#include <chrono>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace engine {

enum LoggingLevel { ANY = 0, DEBUG = 100, INFO = 200, WARN = 300, ERROR = 400, SEVERE = 500 };

class Logger {

public:
  Logger(std::chrono::time_point<std::chrono::high_resolution_clock> start, std::ostream& ostream);

  LoggingLevel getLevelByName(char* levelName);
  void setLoggingLevel(LoggingLevel level);

  void log(LoggingLevel level, const std::string& message);

  // Shortcuts
  void debug(const std::string& message);
  void info(const std::string& message);
  void warn(const std::string& message);
  void error(const std::string& message);
  void severe(const std::string& message);

  template <typename... Args>
  void log(LoggingLevel level, const std::string& message, Args... args) {
    if (level < this->loggingLevel) {
      return;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
    size_t size = 1 + snprintf(nullptr, 0, message.c_str(), args...);
    std::vector<char> buf(size);
    snprintf(buf.data(), size, message.c_str(), args...);
    const std::string formattedMessage(buf.begin(), buf.end());
#pragma clang diagnostic pop

    this->log(level, formattedMessage);
  }

  // Shortcuts
  template <typename... Args>
  void debug(const std::string& message, Args... args) {
    this->log(DEBUG, message, args...);
  };
  template <typename... Args>
  void info(const std::string& message, Args... args) {
    this->log(INFO, message, args...);
  };
  template <typename... Args>
  void warn(const std::string& message, Args... args) {
    this->log(WARN, message, args...);
  };
  template <typename... Args>
  void error(const std::string& message, Args... args) {
    this->log(ERROR, message, args...);
  };
  template <typename... Args>
  void severe(const std::string& message, Args... args) {
    this->log(SEVERE, message, args...);
  };

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  LoggingLevel loggingLevel = LoggingLevel::ANY;
  std::ostream& ostream;

  const std::string levelToString(LoggingLevel level);
  std::chrono::milliseconds getTimeDelta();
};
}

#endif
