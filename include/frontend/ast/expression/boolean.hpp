#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class BooleanExpression : public Expression {
private:
  Position position;

  bool value;

public:
  explicit BooleanExpression(const Position &position, const bool value);

  llvm::Value *codegen() override;

  bool get_value();

  Position *get_position() override{
    return &position;
  };

  string to_string() const override;
  string to_string_tree() const override;
};
