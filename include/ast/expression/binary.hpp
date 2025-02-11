#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "codegen/context.hpp"
#include "lexer/position.hpp"
#include "lexer/token/token.hpp"
#include "types/type.hpp"

using namespace std;

class BinaryExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type = nullptr;

  Token op;

  unique_ptr<Expression> left;
  unique_ptr<Expression> right;

 public:
  BinaryExpression(const Position &position, const Token &op,
                   unique_ptr<Expression> left, unique_ptr<Expression> right)
      : position(position),
        op(op),
        left(std::move(left)),
        right(std::move(right)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
