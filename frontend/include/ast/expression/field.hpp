#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "position.hpp"
#include "types/struct.hpp"
#include "types/type.hpp"

using namespace std;

class FieldExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;
  shared_ptr<StructType> struct_type;

 public:
  shared_ptr<Expression> expression;
  shared_ptr<IdentifierExpression> field;

  explicit FieldExpression(const Position &position,
                           const shared_ptr<Expression> &expression,
                           const shared_ptr<IdentifierExpression> &field)
      : position(position), expression(expression), field(field) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }
  shared_ptr<StructType> get_struct_type() const { return struct_type; }

  string to_string() const override;
  string to_string_tree() const override;
};
