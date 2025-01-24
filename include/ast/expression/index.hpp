#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class IndexExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<VarRefExpression> variable;
  unique_ptr<Expression> index;

 public:
  explicit IndexExpression(const Position &position,
                           const shared_ptr<Type> &type,
                           unique_ptr<VarRefExpression> variable,
                           unique_ptr<Expression> index)
      : position(position),
        type(type),
        variable(std::move(variable)),
        index(std::move(index)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
