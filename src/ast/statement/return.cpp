#include "ast/statement/return.hpp"

#include <llvm/IR/Value.h>

#include <memory>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *ReturnStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  if (Value *value = expression->codegen(context)) {
    auto function = context->builder->GetInsertBlock()->getParent();
    auto return_type = function->getReturnType();

    value = Codegen::cast_type(context, value, return_type);
    if (!value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    }

    context->builder->CreateRet(value);
    return value;
  }

  Logger::error("Failed to generate low level code for return value",
                LogTypes::Error::UNKNOWN, expression->get_position());
  return nullptr;
}

void ReturnStatement::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);
}

void ReturnStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);

  type = expression->get_type();
}

string ReturnStatement::to_string() const {
  return "return " + expression->to_string();
}

string ReturnStatement::to_string_tree() const {
  return "ReturnStatement(type: " + type->to_string_tree() +
         ", expression: " + expression->to_string_tree() + ")";
}
