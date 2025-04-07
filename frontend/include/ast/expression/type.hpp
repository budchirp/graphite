#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class TypeExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  explicit TypeExpression(const Position &position,
                          const shared_ptr<Type> &type)
      : position(position), type(type) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  shared_ptr<Type> resolve_types(const shared_ptr<ProgramContext> &context,
                                 const shared_ptr<Type> &type) const;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
