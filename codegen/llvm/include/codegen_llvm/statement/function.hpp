#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/function.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class FunctionStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<FunctionStatement> statement;

 public:
  explicit FunctionStatementCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<FunctionStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
  llvm::Function* codegen_function() const;
};
