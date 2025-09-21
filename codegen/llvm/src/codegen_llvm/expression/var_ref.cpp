#include "codegen_llvm/expression/var_ref.hpp"

#include "codegen_llvm/options.hpp"

llvm::Value *VarRefExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value *VarRefExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions> &options) const {
  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->name);

  return (options && !options->load_variable)
             ? context->get_variable(expression->name)
             : context->get_variable_value(variable);
}
