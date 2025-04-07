#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement/block.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class IfExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  shared_ptr<Expression> condition;
  shared_ptr<BlockStatement> consequence;
  shared_ptr<BlockStatement> alternative;

  explicit IfExpression(const Position &position,
                        const shared_ptr<Expression> &condition,
                        const shared_ptr<BlockStatement> &consequence,
                        const shared_ptr<BlockStatement> &alternative)
      : position(position),
        condition(condition),
        consequence(consequence),
        alternative(alternative) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
