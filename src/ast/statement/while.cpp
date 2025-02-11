#include "ast/statement/while.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/ConvertUTF.h>

#include <memory>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

using namespace llvm;

Value *WhileStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto condition_block =
      BasicBlock::Create(*context->llvm_context, "loop.condition");
  auto body_block = BasicBlock::Create(*context->llvm_context, "loop.body");
  auto end_block = BasicBlock::Create(*context->llvm_context, "loop.end");

  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), condition_block);
  context->builder->SetInsertPoint(condition_block);

  auto condition_value = condition->codegen(context);
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in while statement",
        LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  context->builder->CreateCondBr(condition_value, body_block, end_block);

  parent_function->insert(parent_function->end(), body_block);
  context->builder->SetInsertPoint(body_block);
  body->codegen(context);

  context->builder->CreateBr(condition_block);

  parent_function->insert(parent_function->end(), end_block);
  context->builder->SetInsertPoint(end_block);

  return Constant::getNullValue(llvm::Type::getVoidTy(*context->llvm_context));
}

void WhileStatement::validate(const shared_ptr<ProgramContext> &context) {
  condition->validate(context);
  if (!TypeHelper::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  body->validate(context);
}

void WhileStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  condition->resolve_types(context);

  body->resolve_types(context);
}

string WhileStatement::to_string() const {
  return "while (" + condition->to_string() + ") " + body->to_string();
}

string WhileStatement::to_string_tree() const {
  return "WhileStatement(condition: " + condition->to_string_tree() +
         ", body: " + body->to_string_tree() + ")";
}
