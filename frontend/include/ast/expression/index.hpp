#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class IndexExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  shared_ptr<IdentifierExpression> identifier;
  shared_ptr<Expression> index;

  explicit IndexExpression(const Position &position,
                           const shared_ptr<IdentifierExpression> &identifier,
                           const shared_ptr<Expression> &index)
      : position(position), identifier(identifier), index(index) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
