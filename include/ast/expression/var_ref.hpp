#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "types/type.hpp"
#include "ast/expression.hpp"
#include "lexer/position.hpp"

using namespace std;

class VarRefExpression : public Expression {
private:
  Position position;

  shared_ptr<Type> type;

  string name;

public:

  explicit VarRefExpression(const Position &position, const shared_ptr<Type> &type,
                                const string &name)
      : position(position), type(type), name(name) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  string get_name() const { return name; };

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
