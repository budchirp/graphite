#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/position.hpp"
#include "lexer/token/token.hpp"
#include "types/type.hpp"

using namespace std;

class UnaryExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  Token op;

  unique_ptr<VarRefExpression> expression;

 public:
  explicit UnaryExpression(const Position &position, const Token &op,
                           unique_ptr<VarRefExpression> expression)
      : position(position), op(op), expression(std::move(expression)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
