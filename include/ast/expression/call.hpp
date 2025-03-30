#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/expression.hpp"
#include "identifier.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class CallExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<IdentifierExpression> name;
  vector<unique_ptr<Expression>> arguments;

 public:
  explicit CallExpression(const Position &position,
                          unique_ptr<IdentifierExpression> name,
                          vector<unique_ptr<Expression>> arguments)
      : position(position),
        name(std::move(name)),
        arguments(std::move(arguments)) {};

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
