#pragma once

#include <memory>

#include "argman.hpp"
#include "env/env.hpp"

using namespace std;

extern shared_ptr<Env> env;

class CompileCommand : public ArgMan::Command {
 public:
  CompileCommand(const string &name, const string &description)
      : ArgMan::Command(name, description) {}

  void execute() override;
};
