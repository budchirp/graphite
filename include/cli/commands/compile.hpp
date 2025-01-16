#pragma once

#include "argman.hpp"

using namespace std;

class CompileCommand : public ArgMan::Command {
 public:
  explicit CompileCommand(const string &name, const string &description)
      : ArgMan::Command(name, description) {}

  void execute() override;
};
