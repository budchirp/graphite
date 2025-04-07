#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/return.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class ReturnStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<ReturnStatement> statement;

 public:
  explicit ReturnStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<ReturnStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
