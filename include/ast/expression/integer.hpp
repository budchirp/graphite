#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "types/type.hpp"
#include "ast/expression.hpp"
#include "lexer/position.hpp"

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

  llvm::Value *codegen() override;

  long long get_value() { return value; };

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
