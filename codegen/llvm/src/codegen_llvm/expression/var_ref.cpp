#include "codegen_llvm/expression/var_ref.hpp"
#include "codegen_llvm/utils.hpp"

llvm::Value *VarRefExpressionCodegen::codegen() const {
  auto variable =
      context->get_env()->get_current_scope()->get_variable(expression->name);
  // if (!variable->is_initialized) {
  //   Logger::error("Access to uninitialized variable",
  //                 LogTypes::Error::UNDEFINED, &position);
  //   return nullptr;
  // }

  auto value = context->get_variable(expression->name);

  return (variable->is_global || variable->is_mutable)
             ? context->builder->CreateLoad(
                   LLVMCodegenUtils::type_to_llvm_type(context,expression->get_type()),
                   value, "load")
             : value;
}
