#pragma once

#include <memory>
#include <string>

#include "frontend/lexer/position.hpp"
#include "logger/log_types.hpp"

using namespace std;

class Logger {
private:
  shared_ptr<Position> position;

public:  
  Logger(shared_ptr<Position> position);

  void log(const string &message, LogTypes::Log type);
  void warn(const string &message, LogTypes::Warn type);
  void error(const string &message, LogTypes::Error type);

  static void log(const string &message);
  static void warn(const string &message);
  static void error(const string &message);
};
