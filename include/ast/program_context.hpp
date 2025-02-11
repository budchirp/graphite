#pragma once

#include <memory>
#include <string>

#include "semantic/env/env.hpp"

using namespace std;

class ProgramContext {
 private:
  string name;

  shared_ptr<Env> env;

 public:
  explicit ProgramContext(const string &name, const shared_ptr<Env> &env)
      : name(name), env(env) {};

  string get_name() const { return name; }

  shared_ptr<Env> get_env() { return env; }
};
