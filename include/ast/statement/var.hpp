#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class VarStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<IdentifierExpression> name;
  unique_ptr<TypeExpression> variable_type;
  unique_ptr<Expression> expression;

 public:
  explicit VarStatement(const Position &position,const shared_ptr<Type> &type,
                        unique_ptr<IdentifierExpression> name,
                        unique_ptr<TypeExpression> variable_type,
                        unique_ptr<Expression> expression)
      : position(position), type(type),
        name(std::move(name)),
        variable_type(std::move(variable_type)),
        expression(std::move(expression)) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
