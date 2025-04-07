#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "position.hpp"

using namespace std;

class ReturnStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  shared_ptr<Expression> expression;

  explicit ReturnStatement(const Position &position,
                           const shared_ptr<Expression> &expression)
      : position(position), expression(expression) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
