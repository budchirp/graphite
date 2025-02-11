#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>
#include <utility>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "ast/statement/block.hpp"
#include "ast/statement/var.hpp"
#include "lexer/position.hpp"
#include "types/void.hpp"

using namespace std;

class WhileStatement : public Statement {
 private:
  Position position;

  unique_ptr<Expression> condition;

  unique_ptr<BlockStatement> body;

 public:
  explicit WhileStatement(const Position &position,
                          unique_ptr<Expression> condition,
                          unique_ptr<BlockStatement> body)
      : position(position),
        condition(std::move(condition)),
        body(std::move(body)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  string to_string() const override;
  string to_string_tree() const override;
};
