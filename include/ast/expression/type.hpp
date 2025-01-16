#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class TypeExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  explicit TypeExpression(const Position &position, shared_ptr<Type> type)
      : position(position), type(type) {};

  llvm::Value *codegen() const override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
