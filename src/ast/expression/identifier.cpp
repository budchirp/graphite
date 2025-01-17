#include "ast/expression/identifier.hpp"

#include <llvm/IR/Value.h>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *IdentifierExpression::codegen(const shared_ptr<CodegenContext> &context) const {
  auto it = context->value_map.find(value);
  if (it == context->value_map.end()) {
    Logger::error("Undefined variable `" + value + "`",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  return it->second;
}

void IdentifierExpression::analyze(
    const shared_ptr<ProgramContext> &context) {
  auto var = context->get_env()->get_symbol(value);
  if (!var) {
    Logger::error("Undefined variable `" + value + "`", LogTypes::Error::UNDEFINED, &position);
  }
}

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(type: " + (type ? type->to_string_tree() : "'variable'") + ", value: '" + value + "')";
}
