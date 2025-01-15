#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>
#include <memory>

#include "ast/statement/return.hpp"
#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *ReturnStatement::codegen() {
  if (Value *value = expression->codegen()) {
    auto function = builder->GetInsertBlock()->getParent();
    auto return_type = function->getReturnType();

    builder->CreateRet(Codegen::cast_type(value, return_type));
    return value;
  } else {
    Logger::error("Failed to generate return value", LogTypes::Error::UNKNOWN,
                  &position);
    return nullptr;
  }
}

string ReturnStatement::to_string() const {
  return "return " + expression->to_string();
}

string ReturnStatement::to_string_tree() const {
  return "ReturnStatement(expression: " + expression->to_string_tree() + ")";
}
