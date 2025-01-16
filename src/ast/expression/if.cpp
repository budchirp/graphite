
#include "ast/expression/if.hpp"

#include <llvm/IR/Value.h>

#include <memory>
#include <sstream>
#include <string>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;
using namespace std;

Value *IfExpression::codegen() const {
  auto condition_value = condition->codegen();
  if (!condition_value) {
    Logger::error(
        "Failed to generate low level code for condition in if expression",
        LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  if (!condition_value->getType()->isIntegerTy(1)) {
    Logger::error("Condition must be a boolean", LogTypes::Error::TYPE_MISMATCH,
                  &position);
    return nullptr;
  }

  auto parent_function = context->builder->GetInsertBlock()->getParent();

  auto then_block = BasicBlock::Create(*context->llvm_context, "then", parent_function);
  auto else_block = BasicBlock::Create(*context->llvm_context, "else");
  auto merge_block = BasicBlock::Create(*context->llvm_context, "if");

  context->builder->CreateCondBr(condition_value, then_block, else_block);

  context->builder->SetInsertPoint(then_block);
  auto then_value = consequence->codegen();
  context->builder->CreateBr(merge_block);
  then_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), else_block);

  context->builder->SetInsertPoint(else_block);
  auto else_value = alternative ? alternative->codegen() : nullptr;
  context->builder->CreateBr(merge_block);
  else_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), merge_block);
  context->builder->SetInsertPoint(merge_block);

  if (else_value && (then_value->getType() == else_value->getType())) {
    PHINode *phi =
        context->builder->CreatePHI(then_value->getType(), 2, "iftmp");
    phi->addIncoming(then_value, then_block);
    phi->addIncoming(else_value, else_block);

    return phi;
  }

  return then_value;
}

string IfExpression::to_string() const {
  ostringstream result;
  result << "if (" << condition->to_string() << ") "
         << consequence->to_string();

  if (alternative) {
    result << " else " << alternative->to_string();
  }

  return result.str();
}

string IfExpression::to_string_tree() const {
  ostringstream result;
  result << "IfExpression(condition: " << condition->to_string_tree()
         << ", consequence: " << consequence->to_string_tree()
         << ", alternative: " << alternative->to_string_tree() << ")";
  return result.str();
}
