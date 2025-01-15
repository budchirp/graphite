#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/block.hpp"

using namespace llvm;

Value *BlockStatement::codegen() {
  Value *last = nullptr;
  for (const auto &statement : statements) {
    last = statement->codegen();
    if (builder->GetInsertBlock()->getTerminator()) {
      break;
    }
  }

  return last;
}

BasicBlock *BlockStatement::codegen_block(Function *parent,
                                          const string &name) {
  auto block = BasicBlock::Create(*context, name, parent);
  builder->SetInsertPoint(block);

  codegen();

  return block;
}
