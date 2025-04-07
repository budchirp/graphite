#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "lexer/token/token.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class BinaryExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type = nullptr;

 public:
  Token op;

  shared_ptr<Expression> left;
  shared_ptr<Expression> right;

  BinaryExpression(const Position &position, const Token &op,
                   const shared_ptr<Expression> &left,
                   const shared_ptr<Expression> &right)
      : position(position), op(op), left(left), right(right) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
