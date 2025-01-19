#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "token/token.hpp"
#include "types/type.hpp"

using namespace std;

class UnaryExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  Token op;

  unique_ptr<Expression> expression;

 public:
  explicit UnaryExpression(const Position &position,
                           const shared_ptr<Type> &type, const Token &op,
                           unique_ptr<Expression> expression)
      : position(position),
        type(type),
        op(op),
        expression(std::move(expression)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
