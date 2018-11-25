#include "Logger.hpp"

namespace engine {

Logger::Logger(std::chrono::time_point<std::chrono::high_resolution_clock> start, std::ostream& ostream)
    : start(start), ostream(ostream) {
}

void Logger::setLoggingLevel(LoggingLevel level) {
  this->loggingLevel = level;
  this->info(std::string("Logging level set to ") + this->levelToString(level) + std::string("."));
}

void Logger::log(LoggingLevel level, const std::string& message) {
  if (level < this->loggingLevel) {
    return;
  }
  this->ostream << "[" << std::fixed << std::setprecision(3) << std::setw(10) << this->getTimeDelta().count() / 1000.f
                << "] [" << this->levelToString(level) << "] " << message << std::endl;
}

void Logger::debug(const std::string& message) {
  this->log(DEBUG, message);
}

void Logger::info(const std::string& message) {
  this->log(INFO, message);
}

void Logger::warn(const std::string& message) {
  this->log(WARN, message);
}

void Logger::error(const std::string& message) {
  this->log(ERROR, message);
}

void Logger::severe(const std::string& message) {
  this->log(SEVERE, message);
}

LoggingLevel Logger::getLevelByName(char* levelName) {
  int levelNameLen = strlen(levelName);
  if (strncmp(levelName, "DEBUG", levelNameLen) == 0) {
    return DEBUG;
  }
  if (strncmp(levelName, "INFO", levelNameLen) == 0) {
    return INFO;
  }
  if (strncmp(levelName, "WARN", levelNameLen) == 0) {
    return WARN;
  }
  if (strncmp(levelName, "ERROR", levelNameLen) == 0) {
    return ERROR;
  }
  if (strncmp(levelName, "SEVERE", levelNameLen) == 0) {
    return SEVERE;
  }
  return ANY;
}

const std::string Logger::levelToString(LoggingLevel level) {
  switch (level) {
  case ANY:
    return "ANY";
  case DEBUG:
    return "DEBUG";
  case INFO:
    return "INFO";
  case WARN:
    return "WARN";
  case ERROR:
    return "ERROR";
  case SEVERE:
    return "SEVERE";
  default:
    return "?";
  }
}

std::chrono::milliseconds Logger::getTimeDelta() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->start);
}
}