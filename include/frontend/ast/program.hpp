#pragma once

#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/statement.hpp"

using namespace std;

class Program : public Statement {
private:
  vector<unique_ptr<Statement>> statements;

public:
  explicit Program(vector<unique_ptr<Statement>> statements);
  ~Program() override = default;

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
