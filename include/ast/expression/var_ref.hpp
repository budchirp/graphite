#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "ast/expression/identifier.hpp"
#include "types/type.hpp"
#include "ast/expression.hpp"
#include "lexer/position.hpp"

using namespace std;

class VarRefExpression : public Expression {
private:
  Position position;

  shared_ptr<Type> type;

public:
  unique_ptr<IdentifierExpression> identifier;

  explicit VarRefExpression(const Position &position, const shared_ptr<Type> &type,
                                unique_ptr<IdentifierExpression> identifier)
      : position(position), type(type), identifier(std::move(identifier)) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  string get_value() const { return identifier->get_value(); };

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
