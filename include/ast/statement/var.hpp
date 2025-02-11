#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"
#include "types/void.hpp"

using namespace std;

class VarStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  bool is_mutable;
  bool is_initialized;

  unique_ptr<IdentifierExpression> name;
  unique_ptr<TypeExpression> variable_type;
  unique_ptr<Expression> expression;

 public:
  explicit VarStatement(const Position &position, const bool &is_mutable,
                        const bool &is_initialized,
                        unique_ptr<IdentifierExpression> name,
                        unique_ptr<TypeExpression> variable_type,
                        unique_ptr<Expression> expression)
      : position(position),
        is_mutable(is_mutable),
        is_initialized(is_initialized),
        name(std::move(name)),
        variable_type(std::move(variable_type)),
        expression(std::move(expression)) {};

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
