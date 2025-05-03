#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "position.hpp"

using namespace std;

class VarStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  SymbolVisibility::Value visibility;

  bool is_mutable;
  bool is_initialized;

  shared_ptr<IdentifierExpression> name;
  shared_ptr<TypeExpression> variable_type;
  shared_ptr<Expression> expression;

  explicit VarStatement(const Position &position,
                        const SymbolVisibility::Value &visibility,
                        const bool &is_mutable, const bool &is_initialized,
                        const shared_ptr<IdentifierExpression> &name,
                        const shared_ptr<TypeExpression> &variable_type,
                        const shared_ptr<Expression> &expression)
      : position(position),
        visibility(visibility),
        is_mutable(is_mutable),
        is_initialized(is_initialized),
        name(name),
        variable_type(variable_type),
        expression(expression) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return type;
  };

  string to_string() const override;
  string to_string_tree() const override;
};
