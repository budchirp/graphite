#include "ast/statement/while.hpp"

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

Value *WhileStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto loop_condition =
      BasicBlock::Create(*context->llvm_context, "loop.condition");
  auto loop_body = BasicBlock::Create(*context->llvm_context, "loop.body");
  auto loop_end = BasicBlock::Create(*context->llvm_context, "loop.end");

  context->builder->CreateBr(loop_condition);

  parent_function->insert(parent_function->end(), loop_condition);
  context->builder->SetInsertPoint(loop_condition);

  auto condition_value = condition->codegen(context);
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in while statement",
        LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  context->builder->CreateCondBr(condition_value, loop_body, loop_end);

  parent_function->insert(parent_function->end(), loop_body);
  context->builder->SetInsertPoint(loop_body);
  body->codegen(context);

  context->builder->CreateBr(loop_condition);

  parent_function->insert(parent_function->end(), loop_end);
  context->builder->SetInsertPoint(loop_end);

  return Constant::getNullValue(llvm::Type::getVoidTy(*context->llvm_context));
}

void WhileStatement::analyze(const shared_ptr<ProgramContext> &context) {
  condition->analyze(context);
  if (!Analyzer::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  body->analyze(context);
}

string WhileStatement::to_string() const { return "for"; }

string WhileStatement::to_string_tree() const { return "WhileStatement()"; }
