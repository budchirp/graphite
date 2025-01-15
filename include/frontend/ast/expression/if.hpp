#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/statement/block.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class IfExpression : public Expression {
private:
  Position position;

  unique_ptr<Expression> condition;
  unique_ptr<BlockStatement> consequence;
  unique_ptr<BlockStatement> alternative;

public:
  explicit IfExpression(const Position &position,
                        unique_ptr<Expression> condition,
                        unique_ptr<BlockStatement> consequence,
                        unique_ptr<BlockStatement> alternative);

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
