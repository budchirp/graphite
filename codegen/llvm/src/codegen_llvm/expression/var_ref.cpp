#include "codegen_llvm/expression/var_ref.hpp"

#include "codegen_llvm/options.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

llvm::Value *VarRefExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value *VarRefExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions> &options) const {
  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->name);

  auto ptr = context->get_variable(variable);
  if (!ptr) {
    Logger::error("Failed to generate low level code for variable",
                  LogTypes::Error::INTERNAL, expression->get_position());
    return nullptr;
  }

  if (options && !options->load_variable) {
    return ptr;
  } else {
    return context->get_variable_value(variable);
  }
}
