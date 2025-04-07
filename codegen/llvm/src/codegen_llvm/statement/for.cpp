#include "codegen_llvm/statement/for.hpp"

#include "codegen_llvm/codegen.hpp"
#include "logger/logger.hpp"

llvm::Value *ForStatementCodegen::codegen() const {
  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto condition_block =
      llvm::BasicBlock::Create(*context->llvm_context, "loop.condition");
  auto body_block =
      llvm::BasicBlock::Create(*context->llvm_context, "loop.body");
  auto increment_block =
      llvm::BasicBlock::Create(*context->llvm_context, "loop.increment");
  auto end_block = llvm::BasicBlock::Create(*context->llvm_context, "loop.end");

  auto init_value = LLVMCodegen::codegen(context, statement->init);
  if (!init_value) {
    Logger::error(
        "Failed to generate low level code for initializer in for statement",
        LogTypes::Error::INTERNAL, statement->init->get_position());
    return nullptr;
  }
  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), condition_block);
  context->builder->SetInsertPoint(condition_block);

  auto condition_value = LLVMCodegen::codegen(context, statement->condition);
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in for statement",
        LogTypes::Error::INTERNAL, statement->condition->get_position());
    return nullptr;
  }

  context->builder->CreateCondBr(condition_value, body_block, end_block);

  parent_function->insert(parent_function->end(), body_block);
  context->builder->SetInsertPoint(body_block);
  LLVMCodegen::codegen(context, statement->body);

  context->builder->CreateBr(increment_block);

  parent_function->insert(parent_function->end(), increment_block);
  context->builder->SetInsertPoint(increment_block);
  auto increment_value = LLVMCodegen::codegen(context, statement->increment);
  if (!increment_value) {
    Logger::error(
        "Failed to generate low level code for expression in for statement",
        LogTypes::Error::INTERNAL, statement->increment->get_position());
    return nullptr;
  }
  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), end_block);
  context->builder->SetInsertPoint(end_block);

  return llvm::Constant::getNullValue(
      llvm::Type::getVoidTy(*context->llvm_context));
}
