#include "ast/statement/for.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/ConvertUTF.h>

#include <memory>
#include <ostream>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
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

void ForStatement::validate(const shared_ptr<ProgramContext> &context) {
  init->validate(context);

  condition->validate(context);
  if (!TypeHelper::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  increment->validate(context);

  body->validate(context);
}

void ForStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  init->resolve_types(context);
  condition->resolve_types(context);
  increment->resolve_types(context);

  body->resolve_types(context);
}

string ForStatement::to_string() const {
  return "for (" + init->to_string() + "; " + condition->to_string() + "; " +
         increment->to_string() + ") " + body->to_string();
}

string ForStatement::to_string_tree() const {
  return "ForStatement(init: " + init->to_string_tree() +
         ", condition: " + condition->to_string_tree() +
         ", increment: " + increment->to_string_tree() +
         ", body: " + body->to_string_tree() + ")";
}
