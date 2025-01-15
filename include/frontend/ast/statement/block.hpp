#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <memory>
#include <string>
#include <vector>

#include "frontend/ast/statement.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class BlockStatement : public Statement {
private:
  Position position;

  vector<unique_ptr<Statement>> statements;

public:
  explicit BlockStatement(const Position &position,
                          vector<unique_ptr<Statement>> statements);

  llvm::Value *codegen() override;
  llvm::BasicBlock *codegen_block(llvm::Function *parent, const string &name);

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
