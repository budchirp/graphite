#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"

using namespace std;

class IdentifierExpression : public Expression {
private:
  string value;

public:
  explicit IdentifierExpression(const string &value);

  llvm::Value *codegen() override;

  string get_value();

  string to_string() const override;
  string to_string_tree() const override;
};
