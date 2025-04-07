
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/while.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class WhileStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<WhileStatement> statement;

 public:
  explicit WhileStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                 const shared_ptr<WhileStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
