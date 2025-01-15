#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/identifier.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *IdentifierExpression::codegen() {
  auto it = named_values.find(value);
  if (it == named_values.end()) {
    Logger::error("Undefined variable " + value, LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  return it->second;
}
