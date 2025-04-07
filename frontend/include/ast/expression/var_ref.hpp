#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class VarRefExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  string name;

  explicit VarRefExpression(const Position &position, const string &name)
      : position(position), name(name) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
