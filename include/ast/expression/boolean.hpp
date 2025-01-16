#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class BooleanExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  bool value;

 public:
  explicit BooleanExpression(const Position &position, shared_ptr<Type> type,
                             const bool value)
      : position(position), type(type), value(value) {};

  llvm::Value *codegen() const override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  bool get_value() const { return value; };

  string to_string() const override;
  string to_string_tree() const override;
};
