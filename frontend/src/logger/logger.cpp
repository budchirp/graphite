#include "logger/logger.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "colors.hpp"
#include "logger/log_types.hpp"
#include "position.hpp"

void Logger::type_error(const string &message, const Position *position,
                        const shared_ptr<Type> &expected,
                        const shared_ptr<Type> &received) {
  Logger::error(message + " expected `" + expected->to_string() +
                    "` but received `" + received->to_string() + "`",
                LogTypes::Error::TYPE_MISMATCH, position);
}

void Logger::log(const string &message, LogTypes::Log::Value type,
                 const Position *position) {
  string log_type;
  switch (type) {
    case LogTypes::Log::INFO:
      log_type = "info";
      break;
  }

  ostringstream str;
  str << Colors::GRAY << "[log][" << log_type << "] "
      << (position ? position->line : 0) << ":"
      << (position ? position->line : 0) << " " << Colors::WHITE << message
      << endl;

  cout << str.str();
};

void Logger::warn(const string &message, LogTypes::Warn::Value type,
                  const Position *position) {
  string warn_type;
  switch (type) {
    case LogTypes::Warn::SUGGESTION:
      warn_type = "suggestion";
      break;
  }

  ostringstream str;
  str << Colors::GRAY << "[" << Colors::YELLOW << "warning" << Colors::GRAY
      << "][" << warn_type << "] " << (position ? position->line : 0) << ":"
      << (position ? position->line : 0) << " " << Colors::WHITE << message
      << endl;

  cout << str.str();
};

void Logger::error(const string &message, LogTypes::Error::Value type,
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
  str << Colors::GRAY << "[" << Colors::RED << "error" << Colors::GRAY << "]["
      << error_type << "] " << (position ? position->line : 0) << ":"
      << (position ? position->line : 0) << " " << Colors::WHITE << message
      << endl;

  throw runtime_error(str.str());
}

void Logger::log(const string &message, LogTypes::Log::Value type) const {
  log(message, type, position.get());
}
void Logger::warn(const string &message, LogTypes::Warn::Value type) const {
  warn(message, type, position.get());
}
void Logger::error(const string &message, LogTypes::Error::Value type) const {
  error(message, type, position.get());
}

void Logger::log(const string &message) {
  cout << Colors::GRAY << "[log]: " << Colors::WHITE << message << endl;
}
void Logger::warn(const string &message) {
  cout << Colors::GRAY << "[" << Colors::YELLOW << "warning" << Colors::GRAY
       << "]: " << Colors::WHITE << message << endl;
}
void Logger::error(const string &message) {
  ostringstream str;
  str << Colors::GRAY << "[" << Colors::RED << "error" << Colors::GRAY
      << "]: " << Colors::WHITE << message << endl;

  throw runtime_error(str.str());
}
