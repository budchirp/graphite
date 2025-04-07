#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/block.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class BlockStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<BlockStatement> statement;

 public:
  explicit BlockStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                 const shared_ptr<BlockStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
  llvm::BasicBlock* codegen_block(llvm::Function* parent,
                                  const string& name) const;
};
