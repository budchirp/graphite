#include "logger/logger.hpp"

#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "cli/colors.hpp"
#include "lexer/position.hpp"
#include "logger/log_types.hpp"

void Logger::log(const string &message, LogTypes::Log type,
                 const Position *position) {
  string log_type;
  switch (type) {
    case LogTypes::Log::INFO:
      log_type = "info";
      break;
  }

  ostringstream str;
  str << Colors::GRAY << "[log][" << log_type << "] " << (position ? position->line : 0) << ":" << (position ? position->line : 0) << " " << Colors::WHITE << message << endl;

  cout << str.str();
};

void Logger::warn(const string &message, LogTypes::Warn type,
                  const Position *position) {
  string warn_type;
  switch (type) {
    case LogTypes::Warn::SUGGESTION:
      warn_type = "suggestion";
      break;
  }

  ostringstream str;
  str << Colors::GRAY << "[" << Colors::YELLOW << "warning" << Colors::GRAY << "][" << warn_type << "] " << (position ? position->line : 0) << ":" << (position ? position->line : 0) << " " << Colors::WHITE << message << endl;

  cout << str.str();
};

void Logger::error(const string &message, LogTypes::Error type,
                   const Position *position) {
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

  ostringstream str;
  str << Colors::GRAY << "[" << Colors::RED << "error" << Colors::GRAY << "][" << error_type << "] " << (position ? position->line : 0) << ":" << (position ? position->line : 0) << " " << Colors::WHITE << message << endl;

  throw runtime_error(str.str());
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
  cout << Colors::GRAY << "[log]: " << Colors::WHITE << message << endl;
}
void Logger::warn(const string &message) {
  cout << Colors::GRAY << "[" << Colors::YELLOW << "warning" << Colors::GRAY << "]: " << Colors::WHITE << message << endl;
}
void Logger::error(const string &message) {
  ostringstream str;
  str << Colors::GRAY << "[" << Colors::YELLOW << "error" << Colors::GRAY << "]: " << Colors::WHITE << message << endl;

  throw runtime_error(str.str());
}
