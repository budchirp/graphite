#include "codegen_llvm/expression/if.hpp"

#include "codegen_llvm/codegen.hpp"
#include "logger/logger.hpp"

llvm::Value *IfExpressionCodegen::codegen() const {
  auto condition_value = LLVMCodegen::codegen(context, expression->condition);
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in if expression",
        LogTypes::Error::INTERNAL, expression->condition->get_position());
    return nullptr;
  }

  if (!condition_value->getType()->isIntegerTy(1)) {
    Logger::error("Condition must be a boolean", LogTypes::Error::TYPE_MISMATCH,
                  expression->condition->get_position());
    return nullptr;
  }

  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto then_block =
      llvm::BasicBlock::Create(*context->llvm_context, "then", parent_function);
  auto else_block = llvm::BasicBlock::Create(*context->llvm_context, "else");
  auto end_block = llvm::BasicBlock::Create(*context->llvm_context, "if");

  context->builder->CreateCondBr(condition_value, then_block, else_block);

  context->builder->SetInsertPoint(then_block);
  auto then_value = LLVMCodegen::codegen(context, expression->consequence);
  context->builder->CreateBr(end_block);
  then_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), else_block);

  context->builder->SetInsertPoint(else_block);
  auto else_value = expression->alternative
                        ? LLVMCodegen::codegen(context, expression->alternative)
                        : nullptr;
  context->builder->CreateBr(end_block);
  else_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), end_block);
  context->builder->SetInsertPoint(end_block);

  if (else_value && (then_value->getType() == else_value->getType())) {
    llvm::PHINode *phi =
        context->builder->CreatePHI(then_value->getType(), 2);
    phi->addIncoming(then_value, then_block);
    phi->addIncoming(else_value, else_block);

    return phi;
  }

  return then_value;
}
