#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "position.hpp"

using namespace std;

class TypeStatement : public Statement {
 private:
  Position position;

 public:
  SymbolVisibility::Value visibility;

  shared_ptr<IdentifierExpression> name;
  shared_ptr<TypeExpression> type;

  explicit TypeStatement(const Position &position,
                         const SymbolVisibility::Value &visibility,
                         const shared_ptr<IdentifierExpression> &name,
                         const shared_ptr<TypeExpression> &type)
      : position(position), visibility(visibility), name(name), type(type) {}

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type->get_type(); };

  string to_string() const override;
  string to_string_tree() const override;
};
