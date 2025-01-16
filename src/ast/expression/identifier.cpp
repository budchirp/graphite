#include <llvm/IR/Value.h>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"
#include "ast/expression/identifier.hpp"

using namespace llvm;

Value *IdentifierExpression::codegen() const {
  auto it = named_values.find(value);
  if (it == named_values.end()) {
    Logger::error("Undefined variable `" + value + "`",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  return it->second;
}

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(value: '" + value + "')";
}
