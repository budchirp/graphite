#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/statement.hpp"
#include "context.hpp"

using namespace std;

class Program {
 private:
  shared_ptr<ProgramContext> context;

 public:
  vector<shared_ptr<Statement>> statements;

  explicit Program(const shared_ptr<ProgramContext> &context)
      : context(context) {};

  void validate();
  void resolve_types();

  shared_ptr<ProgramContext> get_context() const { return context; }

  string to_string() const;
  string to_string_tree() const;
};
