#include "codegen_llvm/expression/var_ref.hpp"

#include "codegen_llvm/options.hpp"

llvm::Value *VarRefExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value *VarRefExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions> &options) const {
  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->name);

  auto var_address = context->get_variable(variable);
  if (!var_address) return nullptr;

  if (options && !options->load_variable) {
    return var_address;
  } else {
    return context->get_variable_value(variable);
  }
}
