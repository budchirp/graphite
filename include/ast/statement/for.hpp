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

class ForStatement : public Statement {
 private:
  Position position;

  unique_ptr<Statement> init;
  unique_ptr<Expression> condition;
  unique_ptr<Expression> increment;

  unique_ptr<BlockStatement> body;

 public:
  explicit ForStatement(const Position &position, unique_ptr<VarStatement> init,
                        unique_ptr<Expression> condition,
                        unique_ptr<Expression> increment,
                        unique_ptr<BlockStatement> body)
      : position(position),
        init(std::move(init)),
        condition(std::move(condition)),
        increment(std::move(increment)),
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
