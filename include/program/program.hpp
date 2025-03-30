#pragma once

#include <memory>
#include <string>
#include <vector>

#include "context.hpp"
#include "ast/statement.hpp"

using namespace std;

class Program {
 private:
  shared_ptr<ProgramContext> context;

  vector<unique_ptr<Statement>> statements;

 public:
  explicit Program(const shared_ptr<ProgramContext> &context)
      : context(context) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const;

  void validate();
  void resolve_types();

  shared_ptr<ProgramContext> get_context() const { return context; }

  void add_statement(unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }

  string to_string() const;
  string to_string_tree() const;
};
