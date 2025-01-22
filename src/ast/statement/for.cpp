#include "ast/statement/for.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/ConvertUTF.h>

#include <memory>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "types/boolean.hpp"

using namespace llvm;

Value *ForStatement::codegen(const shared_ptr<CodegenContext> &context) const {
  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto condition_block =
      BasicBlock::Create(*context->llvm_context, "loop.condition");
  auto body_block = BasicBlock::Create(*context->llvm_context, "loop.body");
  auto increment_block =
      BasicBlock::Create(*context->llvm_context, "loop.increment");
  auto end_block = BasicBlock::Create(*context->llvm_context, "loop.end");

  auto init_value = init->codegen(context);
  if (!init_value) {
    Logger::error(
        "Failed to generate low level code for initializer in for statement",
        LogTypes::Error::INTERNAL, init->get_position());
    return nullptr;
  }
  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), condition_block);
  context->builder->SetInsertPoint(condition_block);

  auto condition_value = condition->codegen(context);
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in for statement",
        LogTypes::Error::INTERNAL, condition->get_position());
    return nullptr;
  }

  context->builder->CreateCondBr(condition_value, body_block, end_block);

  parent_function->insert(parent_function->end(), body_block);
  context->builder->SetInsertPoint(body_block);
  body->codegen(context);

  context->builder->CreateBr(increment_block);

  parent_function->insert(parent_function->end(), increment_block);
  context->builder->SetInsertPoint(increment_block);
  auto increment_value = increment->codegen(context);
  if (!increment_value) {
    Logger::error(
        "Failed to generate low level code for expression in for statement",
        LogTypes::Error::INTERNAL, increment->get_position());
    return nullptr;
  }
  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), end_block);
  context->builder->SetInsertPoint(end_block);

  return Constant::getNullValue(llvm::Type::getVoidTy(*context->llvm_context));
}

void ForStatement::analyze(const shared_ptr<ProgramContext> &context) {
  init->analyze(context);

  condition->analyze(context);
  if (!Analyzer::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  increment->analyze(context);

  body->analyze(context);
}

string ForStatement::to_string() const { return "for"; }

string ForStatement::to_string_tree() const { return "ForStatement()"; }
