#include "codegen_llvm/statement/block.hpp"

#include "codegen_llvm/codegen.hpp"

using namespace std;

llvm::Value *BlockStatementCodegen::codegen() const {
  llvm::Value *last_statement = nullptr;
  for (const auto &child_statement : statement->statements) {
    last_statement = LLVMCodegen::codegen(context, child_statement);
    if (context->builder->GetInsertBlock()->getTerminator()) {
      break;
    }
  }

  return last_statement;
}

llvm::BasicBlock *BlockStatementCodegen::codegen_block(llvm::Function *parent,
    const string &name) const {
  auto block = llvm::BasicBlock::Create(*context->llvm_context, name, parent);
  context->builder->SetInsertPoint(block);

  codegen();

  return block;
}
