#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "lexer/position.hpp"
#include "types/array.hpp"
#include "types/type.hpp"

using namespace std;

class ArrayExpression : public Expression {
 private:
  Position position;

  shared_ptr<ArrayType> type;

  vector<unique_ptr<Expression>> values;

 public:
  explicit ArrayExpression(const Position &position,
                           const shared_ptr<ArrayType> &type,
                           vector<unique_ptr<Expression>> values)
      : position(position), type(type), values(std::move(values)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
