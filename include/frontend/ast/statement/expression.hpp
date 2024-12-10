#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/statement.hpp"

using namespace std;

class ExpressionStatement : public Statement {
private:
  unique_ptr<Expression> expression;

public:
  explicit ExpressionStatement(unique_ptr<Expression> expression);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};