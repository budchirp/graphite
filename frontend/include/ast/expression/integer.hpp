#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "position.hpp"
#include "types/int.hpp"
#include "types/type.hpp"

using namespace std;

class IntegerExpression : public Expression {
 private:
  Position position;

  shared_ptr<IntType> type;

 public:
  string value;

  explicit IntegerExpression(const Position &position, const string &value)
      : position(position), value(value) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context,
                     const shared_ptr<IntType> &destination_type);
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override {
    this->type = dynamic_pointer_cast<IntType>(type);
  }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
