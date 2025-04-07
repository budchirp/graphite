#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/for.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class ForStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<ForStatement> statement;

 public:
  explicit ForStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                               const shared_ptr<ForStatement> &statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};
