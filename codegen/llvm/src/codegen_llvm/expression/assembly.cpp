#include "codegen_llvm/expression/assembly.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"

llvm::Value *AssemblyExpressionCodegen::codegen() const {
  auto llvm_type =
      static_cast<llvm::FunctionType *>(LLVMCodegenUtils::type_to_llvm_type(
          context, expression->get_function_type()));

  vector<llvm::Value *> arguments;

  ostringstream constraints_oss;
  for (const auto &[identifier, variable] : expression->output) {
    auto value = context->get_variable(
        variable->value);
    arguments.push_back(value);

    constraints_oss << identifier->value << ",";
  }

  auto scope = context->get_env()->get_current_scope();

  for (const auto &[identifier, variable] : expression->input) {
    auto value = context->get_variable_value(
        scope->get_variable(variable->value));
    arguments.push_back(value);

    constraints_oss << identifier->value << ",";
  }

  for (const auto &clobber : expression->clobbered) {
    constraints_oss << "~{" << clobber->value << "},";
  }

  auto constraints = constraints_oss.str();
  if (constraints.ends_with(",")) {
    constraints.pop_back();
  }

  auto inline_assembly = llvm::InlineAsm::get(
      llvm_type, expression->assembly->value, constraints, true, false);

  return context->builder->CreateCall(inline_assembly, arguments);
}
