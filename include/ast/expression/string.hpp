#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "types/type.hpp"
#include "ast/expression.hpp"
#include "lexer/position.hpp"

using namespace std;

class StringExpression : public Expression {
private:
  Position position;

  shared_ptr<Type> type;

  string value;

public:
  explicit StringExpression(const Position &position, shared_ptr<Type> type,
                            const string &value)
      : position(position), type(type), value(value) {};

  llvm::Value *codegen() override;

  string get_value() { return value; };

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
