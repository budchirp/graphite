#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/expression.hpp"
#include "identifier.hpp"

using namespace std;

class CallExpression : public Expression {
private:
  unique_ptr<IdentifierExpression> name;
  vector<unique_ptr<Expression>> arguments;

public:
  explicit CallExpression(unique_ptr<IdentifierExpression> name,
                 vector<unique_ptr<Expression>> arguments);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
