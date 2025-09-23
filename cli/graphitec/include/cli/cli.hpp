#pragma once

#include <memory>

#include "argman.hpp"

using namespace std;
using namespace ArgMan;

class CLI {
 private:
  shared_ptr<CommandLineParser> parser;

 public:
  explicit CLI();

  void parse(int argc, char *argv[]);
};
