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

  vector<unique_ptr<Statement>> statements;

public:
  explicit BlockStatement(const Position &position,
                          vector<unique_ptr<Statement>> statements)
      : position(position), statements(std::move(statements)) {};

  llvm::Value *codegen() override;
  llvm::BasicBlock *codegen_block(llvm::Function *parent, const string &name);

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
