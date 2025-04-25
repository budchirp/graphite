#pragma once

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class Logger {
 private:
  shared_ptr<Position> position;

 public:
  explicit Logger(const shared_ptr<Position> &position) : position(position) {};

  static void type_error(const string &message, const Position *position,
                         const shared_ptr<Type> &expected,
                         const shared_ptr<Type> &received);

  static void log(const string &message, LogTypes::Log::Value type,
                  const Position *position);
  static void warn(const string &message, LogTypes::Warn::Value type,
                   const Position *position);
  static void error(const string &message, LogTypes::Error::Value type,
                    const Position *position);

  void log(const string &message, LogTypes::Log::Value type) const;
  void warn(const string &message, LogTypes::Warn::Value type) const;
  void error(const string &message, LogTypes::Error::Value type) const;

  static void log(const string &message);
  static void warn(const string &message);
  static void error(const string &message);
};
