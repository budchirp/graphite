#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"

using namespace std;

class BooleanExpression : public Expression {
private:
  bool value;

public:
  explicit BooleanExpression(const bool value);

  llvm::Value *codegen() override;

  bool get_value();

  string to_string() const override;
  string to_string_tree() const override;
};
