#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/include.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class IncludeStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<IncludeStatement> statement;

 public:
  explicit IncludeStatementCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<IncludeStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
