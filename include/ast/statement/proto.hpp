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

  shared_ptr<Type> type;

 public:
  explicit ProtoStatement(
      const Position &position, const shared_ptr<Type> &type,unique_ptr<IdentifierExpression> name,
      vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
          parameters,
      unique_ptr<TypeExpression> return_type)
      : position(position),type(type),
        name(std::move(name)),
        parameters(std::move(parameters)),
        return_type(std::move(return_type)) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const override;
  llvm::Function *codegen_function(const shared_ptr<CodegenContext> &context) const;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  unique_ptr<IdentifierExpression> name;
  vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
      parameters;
  unique_ptr<TypeExpression> return_type;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
