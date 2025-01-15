#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "block.hpp"
#include "frontend/ast/statement.hpp"
#include "frontend/ast/statement/proto.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class FunctionStatement : public Statement {
private:
  Position position;

  unique_ptr<ProtoStatement> proto;
  unique_ptr<BlockStatement> body;

public:
  explicit FunctionStatement(const Position &position,
                             unique_ptr<ProtoStatement> proto,
                             unique_ptr<BlockStatement> body);

  llvm::Value *codegen() override;
  llvm::Function *codegen_function();

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
