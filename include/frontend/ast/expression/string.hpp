#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class StringExpression : public Expression {
private:
  Position position;

  string value;

public:
  explicit StringExpression(const Position &position, const string &value);

  llvm::Value *codegen() override;

  string get_value();

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
