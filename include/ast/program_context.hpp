#pragma once

#include <memory>
#include <string>

#include "env/env.hpp"

using namespace std;

class ProgramContext {
 private:
  string name;

  shared_ptr<Env> env;

 public:
  explicit ProgramContext(const string &name, const shared_ptr<Env> &env) : name(name), env(env) {};

  string get_name() const { return name; }

  void set_env(const shared_ptr<Env> &env) { this->env = env; }
  shared_ptr<Env> get_env() const { return env; }
};
