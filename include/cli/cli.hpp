#pragma once

#include <memory>

#include "argman.hpp"

using namespace std;
using namespace ArgMan;

class CLI {
public:
  shared_ptr<CommandLineParser> parser;

  CLI();
};
