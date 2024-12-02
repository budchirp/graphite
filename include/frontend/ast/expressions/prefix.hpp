#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/token/token.hpp"

using namespace std;

class PrefixExpression : public Expression {
private:
  Token prefix;

  unique_ptr<Expression> right;

public:
  PrefixExpression(Token prefix, unique_ptr<Expression> right);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
