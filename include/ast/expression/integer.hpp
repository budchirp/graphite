#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class IntegerExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  long long value;

 public:
  explicit IntegerExpression(const Position &position, shared_ptr<Type> type,
                             const long long &value)
      : position(position), type(type), value(value) {};

  llvm::Value *codegen() const override;

  long long get_value() const { return value; };

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
