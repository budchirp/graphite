
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/var.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class VarStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<VarStatement> statement;

 public:
  explicit VarStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                               const shared_ptr<VarStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
