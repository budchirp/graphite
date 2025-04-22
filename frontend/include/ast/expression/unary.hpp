#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/token/token.hpp"
#include "position.hpp"
#include "types/type.hpp"

using namespace std;

class UnaryExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  Token op;

  shared_ptr<VarRefExpression> var_ref;

  explicit UnaryExpression(const Position &position, const Token &op,
                           const shared_ptr<VarRefExpression> &var_ref)
      : position(position), op(op), var_ref(var_ref) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
