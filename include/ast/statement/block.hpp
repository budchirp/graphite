#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

#include <memory>
#include <string>
#include <vector>

#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class BlockStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  vector<unique_ptr<Statement>> statements;

 public:
  explicit BlockStatement(const Position &position,
                          vector<unique_ptr<Statement>> statements)
      : position(position), statements(std::move(statements)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;
  llvm::BasicBlock *codegen_block(const shared_ptr<CodegenContext> &context,
                                  llvm::Function *parent,
                                  const string &name) const;
  
  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
