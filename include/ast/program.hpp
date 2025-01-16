#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/statement.hpp"
#include "env/env.hpp"

using namespace std;

class Program {
 private:
  string name;

  shared_ptr<Env> env;

  vector<unique_ptr<Statement>> statements;

 public:
  explicit Program(const string &name, const shared_ptr<Env> &env)
      : name(name), env(env) {};

  llvm::Value *codegen() const;

  string get_name() const { return name; }

  void add_statement(unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }

  void set_env(shared_ptr<Env> env) { this->env = env; }
  shared_ptr<Env> get_env() const {
    return env;
  }

  string to_string() const;
  string to_string_tree() const;
};
