#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "types/type.hpp"
#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "token/token.hpp"

using namespace std;

class PrefixExpression : public Expression {
private:
  Position position;

  shared_ptr<Type> type;

  Token prefix;

  unique_ptr<Expression> right;

public:
  explicit PrefixExpression(const Position &position, shared_ptr<Type> type,
                            const Token &prefix, unique_ptr<Expression> right)
      : position(position), type(type), prefix(prefix),
        right(std::move(right)) {};

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
