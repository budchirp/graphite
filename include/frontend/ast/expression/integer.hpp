#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class IntegerExpression : public Expression {
private:
  Position position;

  int value;

public:
  explicit IntegerExpression(const Position &position, const int &value);

  llvm::Value *codegen() override;

  int get_value();

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
