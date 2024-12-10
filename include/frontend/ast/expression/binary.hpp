#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/token/token.hpp"

using namespace std;

class BinaryExpression : public Expression {
private:
  Token op;

  unique_ptr<Expression> left;
  unique_ptr<Expression> right;

public:
  BinaryExpression(const Token &op, unique_ptr<Expression> left,
                   unique_ptr<Expression> right);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
