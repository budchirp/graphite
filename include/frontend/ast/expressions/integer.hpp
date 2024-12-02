#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"
#include "std/types.hpp"

using namespace std;

class IntegerExpression : public Expression {
private:
  i32 value;

public:
  IntegerExpression(i32 value);

  llvm::Value *codegen() override;

  i32 get_value();

  string to_string() const override;
  string to_string_tree() const override;
};
