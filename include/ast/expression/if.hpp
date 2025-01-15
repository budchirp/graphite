#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "types/type.hpp"
#include "ast/expression.hpp"
#include "ast/statement/block.hpp"
#include "lexer/position.hpp"

using namespace std;

class IfExpression : public Expression {
private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<Expression> condition;
  unique_ptr<BlockStatement> consequence;
  unique_ptr<BlockStatement> alternative;

public:
  explicit IfExpression(const Position &position, shared_ptr<Type> type,
                        unique_ptr<Expression> condition,
                        unique_ptr<BlockStatement> consequence,
                        unique_ptr<BlockStatement> alternative)
      : position(position), type(type),
        condition(std::move(condition)), consequence(std::move(consequence)),
        alternative(std::move(alternative)) {};

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
