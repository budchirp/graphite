#include "ast/expression/if.hpp"

#include <llvm/IR/Value.h>

#include <memory>
#include <sstream>
#include <string>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "types/boolean.hpp"

using namespace llvm;
using namespace std;

Value *IfExpression::codegen(const shared_ptr<CodegenContext> &context) const {
  auto condition_value = condition->codegen(context);
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

  auto then_block =
      BasicBlock::Create(*context->llvm_context, "then", parent_function);
  auto else_block = BasicBlock::Create(*context->llvm_context, "else");
  auto end_block = BasicBlock::Create(*context->llvm_context, "if");

  context->builder->CreateCondBr(condition_value, then_block, else_block);

  context->builder->SetInsertPoint(then_block);
  auto then_value = consequence->codegen(context);
  context->builder->CreateBr(end_block);
  then_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), else_block);

  context->builder->SetInsertPoint(else_block);
  auto else_value = alternative ? alternative->codegen(context) : nullptr;
  context->builder->CreateBr(end_block);
  else_block = context->builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), end_block);
  context->builder->SetInsertPoint(end_block);

  if (else_value && (then_value->getType() == else_value->getType())) {
    PHINode *phi =
        context->builder->CreatePHI(then_value->getType(), 2, "iftmp");
    phi->addIncoming(then_value, then_block);
    phi->addIncoming(else_value, else_block);

    return phi;
  }

  return then_value;
}

void IfExpression::analyze(const shared_ptr<ProgramContext> &context) {
  if (!Analyzer::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on if condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  condition->analyze(context);

  consequence->analyze(context);
  if (alternative) alternative->analyze(context);
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
  result << "IfExpression(type: " + type->to_string_tree() + ", condition: "
         << condition->to_string_tree()
         << ", consequence: " << consequence->to_string_tree()
         << ", alternative: "
         << (alternative ? alternative->to_string_tree() : "empty") << ")";
  return result.str();
}
