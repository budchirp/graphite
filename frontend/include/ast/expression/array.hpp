#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "position.hpp"
#include "types/array.hpp"
#include "types/type.hpp"

using namespace std;

class ArrayExpression : public Expression {
 private:
  Position position;

  shared_ptr<ArrayType> type;

 public:
  vector<shared_ptr<Expression>> values;

  explicit ArrayExpression(const Position &position,
                           const vector<shared_ptr<Expression>> &values)
      : position(position), values(values) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context,
                     const shared_ptr<ArrayType> &destination_type);
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override {
    this->type = dynamic_pointer_cast<ArrayType>(type);
  }
  shared_ptr<Type> get_type() const override { return type; }
  shared_ptr<ArrayType> get_array_type() const { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
