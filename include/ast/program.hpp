#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/program_context.hpp"
#include "ast/statement.hpp"
#include "env/env.hpp"

using namespace std;

class Program {
 private:
  shared_ptr<ProgramContext> context;

  vector<unique_ptr<Statement>> statements;

 public:
  explicit Program(const shared_ptr<ProgramContext> &context)
      : context(context) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const;
  void analyze();

  shared_ptr<ProgramContext> get_context() const { return context; }

  void add_statement(unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }

  void set_env(const shared_ptr<Env> &env) { context->set_env(env); }
  shared_ptr<Env> get_env() const { return context->get_env(); }

  string to_string() const;
  string to_string_tree() const;
};
