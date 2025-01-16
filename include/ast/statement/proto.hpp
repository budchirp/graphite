#pragma once

#include <llvm/IR/Function.h>

#include <memory>
#include <string>
#include <vector>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class ProtoStatement : public Statement {
 private:
  Position position;

 public:
  explicit ProtoStatement(
      const Position &position, unique_ptr<IdentifierExpression> name,
      vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
          parameters,
      unique_ptr<TypeExpression> return_type)
      : position(position),
        name(std::move(name)),
        parameters(std::move(parameters)),
        return_type(std::move(return_type)) {};

  llvm::Value *codegen() const override;
  llvm::Function *codegen_function() const;

  unique_ptr<IdentifierExpression> name;
  vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
      parameters;
  unique_ptr<TypeExpression> return_type;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
