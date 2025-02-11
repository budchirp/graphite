#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class ReturnStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<Expression> expression;

 public:
  explicit ReturnStatement(const Position &position,
                           unique_ptr<Expression> expression)
      : position(position), expression(std::move(expression)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
