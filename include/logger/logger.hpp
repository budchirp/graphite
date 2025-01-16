#pragma once

#include <memory>
#include <string>

#include "lexer/position.hpp"
#include "logger/log_types.hpp"

using namespace std;

class Logger {
 private:
  shared_ptr<Position> position;

 public:
  explicit Logger(shared_ptr<Position> position);

  static void log(const string &message, LogTypes::Log type,
                  const Position *position);
  static void warn(const string &message, LogTypes::Warn type,
                   const Position *position);
  static void error(const string &message, LogTypes::Error type,
                    const Position *position);

  void log(const string &message, LogTypes::Log type);
  void warn(const string &message, LogTypes::Warn type);
  void error(const string &message, LogTypes::Error type);

  static void log(const string &message);
  static void warn(const string &message);
  static void error(const string &message);
};
