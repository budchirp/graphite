#pragma once

#include <llvm/IR/Function.h>

#include <memory>
#include <string>
#include <vector>

#include "ast/expression/string.hpp"
#include "ast/program.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class IncludeStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<StringExpression> module;

  shared_ptr<Program> program;

 public:
  explicit IncludeStatement(const Position &position,
                            unique_ptr<StringExpression> module)
      : position(position), module(std::move(module)), program(nullptr) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
