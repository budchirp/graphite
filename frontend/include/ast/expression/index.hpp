#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class IndexExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  shared_ptr<VarRefExpression> variable;
  shared_ptr<Expression> index;

  explicit IndexExpression(const Position &position,
                           const shared_ptr<VarRefExpression> &variable,
                           const shared_ptr<Expression> &index)
      : position(position), variable(variable), index(index) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
