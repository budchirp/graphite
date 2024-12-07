#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/statements/block.hpp"

using namespace std;

class IfExpression : public Expression {
private:
  unique_ptr<Expression> condition;
  unique_ptr<BlockStatement> consequence;
  unique_ptr<BlockStatement> alternative;

public:
  IfExpression(unique_ptr<Expression> condition,
               unique_ptr<BlockStatement> consequence,
               unique_ptr<BlockStatement> alternative);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
