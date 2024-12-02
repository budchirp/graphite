#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/statement.hpp"
#include "frontend/ast/statements/proto.hpp"
#include "proto.hpp"

using namespace std;

class ExternStatement : public Statement {
private:
  unique_ptr<ProtoStatement> proto;

public:
  ExternStatement(unique_ptr<ProtoStatement> proto);

  llvm::Value *codegen() override;

  string to_string() const override;
  string to_string_tree() const override;
};
