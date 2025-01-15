#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/if.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *IfExpression::codegen() {
  Value *condition_value = condition->codegen();
  if (!condition_value) {
    Logger::error("Failed to generate condition for if expression", LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  if (!condition_value->getType()->isIntegerTy(1)) {
    Logger::error("Condition must be a boolean", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  Function *parent_function = builder->GetInsertBlock()->getParent();

  auto then_block = BasicBlock::Create(*context, "then", parent_function);
  auto else_block = BasicBlock::Create(*context, "else");
  auto merge_block = BasicBlock::Create(*context, "ifcontent");

  builder->CreateCondBr(condition_value, then_block, else_block);

  builder->SetInsertPoint(then_block);
  Value *then_value = consequence->codegen();
  builder->CreateBr(merge_block);
  then_block = builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), else_block);

  builder->SetInsertPoint(else_block);
  Value *else_value = alternative ? alternative->codegen() : nullptr;
  builder->CreateBr(merge_block);
  else_block = builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), merge_block);
  builder->SetInsertPoint(merge_block);

  if (then_value && else_value &&
      then_value->getType() == else_value->getType()) {
    PHINode *phi = builder->CreatePHI(then_value->getType(), 2, "iftmp");
    phi->addIncoming(then_value, then_block);
    phi->addIncoming(else_value, else_block);

    return phi;
  } else {
    return then_value;
  }
}
