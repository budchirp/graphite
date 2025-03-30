#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"
#include "types/void.hpp"

using namespace std;

class IdentifierExpression : public Expression {
 private:
  Position position;

  string value;

 public:
  explicit IdentifierExpression(const Position &position, const string &value)
      : position(position), value(value) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  string get_identifier() const { return value; };

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override {}
  shared_ptr<Type> get_type() const override { return make_shared<VoidType>(); }

  string to_string() const override;
  string to_string_tree() const override;
};
