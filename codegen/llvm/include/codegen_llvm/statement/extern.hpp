#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/extern.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class ExternStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<ExternStatement> statement;

 public:
  explicit ExternStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<ExternStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
