#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/statement.hpp"
#include "frontend/ast/statement/proto.hpp"
#include "frontend/lexer/position.hpp"
#include "proto.hpp"

using namespace std;

class ExternStatement : public Statement {
private:
  Position position;

  unique_ptr<ProtoStatement> proto;

public:
  explicit ExternStatement(const Position &position,
                           unique_ptr<ProtoStatement> proto);

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
