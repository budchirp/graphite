#pragma once

#include <string>

using namespace std;

class Logger {
public:
  static void log(string message);
  static void warn(string message);
  static void error(string message);
};
