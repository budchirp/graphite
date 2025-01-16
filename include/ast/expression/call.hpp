#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>
#include <vector>

#include "ast/expression.hpp"
#include "identifier.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class CallExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<IdentifierExpression> name;
  vector<unique_ptr<Expression>> arguments;

 public:
  explicit CallExpression(const Position &position, shared_ptr<Type> type,
                          unique_ptr<IdentifierExpression> name,
                          vector<unique_ptr<Expression>> arguments)
      : position(position),
        type(type),
        name(std::move(name)),
        arguments(std::move(arguments)) {};

  llvm::Value *codegen() const override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
