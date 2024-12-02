#pragma once

#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/expressions/identifier.hpp"
#include "frontend/ast/statement.hpp"

using namespace std;

class ProtoStatement : public Statement {
public:
  unique_ptr<IdentifierExpression> name;
  vector<unique_ptr<IdentifierExpression>> arguments;

  ProtoStatement(unique_ptr<IdentifierExpression> name,
                 vector<unique_ptr<IdentifierExpression>> arguments);

  llvm::Value *codegen() override;
  llvm::Function *codegen_function();

  string to_string() const override;
  string to_string_tree() const override;
};
