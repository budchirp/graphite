#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/statement.hpp"

using namespace std;

class BlockStatement : public Statement {
private:
  vector<unique_ptr<Statement>> statements;

public:
  explicit BlockStatement(vector<unique_ptr<Statement>> statements);

  llvm::Value *codegen() override;
  llvm::BasicBlock *codegen_block(llvm::Function *parent, const string &name);

  string to_string() const override;
  string to_string_tree() const override;
};
