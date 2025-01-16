#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class ExpressionStatement : public Statement {
 private:
  Position position;

  unique_ptr<Expression> expression;

 public:
  explicit ExpressionStatement(const Position &position,
                               unique_ptr<Expression> expression)
      : position(position), expression(std::move(expression)) {};

  llvm::Value *codegen() const override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
