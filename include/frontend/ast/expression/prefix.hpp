#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/lexer/position.hpp"
#include "frontend/token/token.hpp"

using namespace std;

class PrefixExpression : public Expression {
private:
  Position position;

  Token prefix;

  unique_ptr<Expression> right;

public:
  explicit PrefixExpression(const Position &position, const Token &prefix,
                            unique_ptr<Expression> right);

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
