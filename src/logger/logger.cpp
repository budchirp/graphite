#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "lexer/position.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

Logger::Logger(shared_ptr<Position> position)
    : position(position) {}

void Logger::log(const string &message, LogTypes::Log type, Position *position) {
  string log_type;
  switch (type) {
  case LogTypes::Log::INFO:
    log_type = "info";
    break;
  }

  if (!position) {
    cerr << "[error][unknown position]: " << message << endl;
    return;
  }

  cout << format("[error][{}] {}:{} {}", log_type, position->line,
                 position->column, message)
       << endl;
};

void Logger::warn(const string &message, LogTypes::Warn type, Position *position) {
  string warn_type;
  switch (type) {
  case LogTypes::Warn::SUGGESTION:
    warn_type = "suggestion";
    break;
  }

  if (!position) {
    cerr << "[error][unknown position]: " << message << endl;
    return;
  }

  cout << format("[error][{}] {}:{} {}", warn_type, position->line,
                 position->column, message)
       << endl;
};

void Logger::error(const string &message, LogTypes::Error type, Position *position) {
  string error_type;
  switch (type) {
  case LogTypes::Error::UNDEFINED:
    error_type = "undefined symbol";
    break;

  case LogTypes::Error::UNKNOWN:
    error_type = "unknown error";
    break;

  case LogTypes::Error::INTERNAL:
    error_type = "internal error";
    break;

  case LogTypes::Error::SYNTAX:
    error_type = "syntax error";
    break;

  case LogTypes::Error::TYPE_MISMATCH:
    error_type = "type mismatch";
    break;
  }

  if (!position) {
    cerr << "[error][unknown position]: " << message << endl;
    return;
  }

  auto msg = format("[error][{}] {}:{} {}", error_type, 
                 position->line, position->column, message);

  throw runtime_error(msg);
}

void Logger::log(const string &message, LogTypes::Log type) {
  Logger::log(message, type, position.get());
}
void Logger::warn(const string &message, LogTypes::Warn type) {
  Logger::warn(message, type, position.get());
}
void Logger::error(const string &message, LogTypes::Error type) {
  Logger::error(message, type, position.get());
}

  void Logger::log(const string &message) {
  cout << "[log]: " << message << endl;
}
void Logger::warn(const string &message) {
  cout << "[warn]: " << message << endl;
}
void Logger::error(const string &message) {
  auto msg = "[error]: " + message;

  throw runtime_error(msg);
}
