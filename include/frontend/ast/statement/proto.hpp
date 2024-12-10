#pragma once

#include <llvm/IR/Function.h>
#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/expression/identifier.hpp"
#include "frontend/ast/statement.hpp"

using namespace std;

class ProtoStatement : public Statement {
public:
  unique_ptr<IdentifierExpression> name;
  vector<
      pair<unique_ptr<IdentifierExpression>, unique_ptr<IdentifierExpression>>>
      parameters;
  unique_ptr<IdentifierExpression> return_type;

  explicit ProtoStatement(unique_ptr<IdentifierExpression> name,
                 vector<pair<unique_ptr<IdentifierExpression>,
                             unique_ptr<IdentifierExpression>>>
                     parameters,
                 unique_ptr<IdentifierExpression> return_type);

  llvm::Value *codegen() override;
  llvm::Function *codegen_function();

  string to_string() const override;
  string to_string_tree() const override;
};
