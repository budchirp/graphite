#include "ast/expression/type.hpp"

#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"

llvm::Value *TypeExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return nullptr;
}

void TypeExpression::analyze(const shared_ptr<ProgramContext> &context) {
  if (!context->get_env()->get_type(type->to_string())) {
    Logger::error("Undefined type `" + type->to_string() + "`",
                  LogTypes::Error::UNDEFINED, &position);
  }
}

string TypeExpression::to_string() const { return type->to_string(); }

string TypeExpression::to_string_tree() const {
  return "TypeExpression(type: " + type->to_string_tree() + ")";
}
