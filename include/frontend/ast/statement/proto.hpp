#pragma once

#include <llvm/IR/Function.h>
#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/expression/identifier.hpp"
#include "frontend/ast/statement.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class ProtoStatement : public Statement {
private:
  Position position;

public:
  explicit ProtoStatement(const Position &position,
                          unique_ptr<IdentifierExpression> name,
                          vector<pair<unique_ptr<IdentifierExpression>,
                                      unique_ptr<IdentifierExpression>>>
                              parameters,
                          unique_ptr<IdentifierExpression> return_type);

  llvm::Value *codegen() override;
  llvm::Function *codegen_function();

  unique_ptr<IdentifierExpression> name;
  vector<
      pair<unique_ptr<IdentifierExpression>, unique_ptr<IdentifierExpression>>>
      parameters;
  unique_ptr<IdentifierExpression> return_type;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
