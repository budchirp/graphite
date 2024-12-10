#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "frontend/lexer/position.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

Logger::Logger(shared_ptr<Position> position)
    : position(position) {}

void Logger::log(const string &message, LogTypes::Log type) {
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

  cout << format("[error][{}] {}:{} {}", log_type, position->column,
                 position->line, message)
       << endl;
};

void Logger::warn(const string &message, LogTypes::Warn type) {
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

  cout << format("[error][{}] {}:{} {}", warn_type, position->column,
                 position->line, message)
       << endl;
};

void Logger::error(const string &message, LogTypes::Error type) {
  string error_type;
  switch (type) {
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
                 position->column, position->line, message);

  throw runtime_error(msg);
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
