#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "semantic/env/env.hpp"

using namespace std;

class ProgramContext {
 private:
  string module_name;

  filesystem::path path;

  shared_ptr<Env> env;

 public:
  explicit ProgramContext(const string &module_name,
                          const filesystem::path &path,
                          const shared_ptr<Env> &env)
      : module_name(module_name), path(path), env(env) {};

  string get_module_name() const { return module_name; }
  filesystem::path get_path() const { return path; }

  shared_ptr<Env> get_env() { return env; }
};
