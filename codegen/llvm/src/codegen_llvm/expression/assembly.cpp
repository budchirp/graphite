#include "codegen_llvm/expression/assembly.hpp"

#include <sstream>
#include <vector>

#include "codegen_llvm/utils.hpp"

llvm::Value *AssemblyExpressionCodegen::codegen() const {
  auto env = context->get_env();
  auto scope = env->get_current_scope();

  vector<llvm::Value *> arguments;

  ostringstream output_constraints;
  for (const auto &[constraint, variable] : expression->output) {
    arguments.push_back(
        context->get_variable(scope->get_variable(variable->value)));

    if (output_constraints.tellp() > 0) {
      output_constraints << ",";
    }
    output_constraints << constraint->value;
  }

  ostringstream input_constraints;
  for (const auto &[constraint, variable] : expression->input) {
    arguments.push_back(
        context->get_variable_value(scope->get_variable(variable->value)));

    if (input_constraints.tellp() > 0) {
      input_constraints << ",";
    }
    input_constraints << constraint->value;
  }

  ostringstream clobber_constraints;
  for (const auto &clobber : expression->clobbered) {
    if (clobber_constraints.tellp() > 0) {
      clobber_constraints << ",";
    }
    clobber_constraints << "~{" << clobber->value << "}";
  }

  string constraints = output_constraints.str();
  if (!input_constraints.str().empty()) {
    if (!constraints.empty()) constraints += ",";
    constraints += input_constraints.str();
  }

  if (!clobber_constraints.str().empty()) {
    if (!constraints.empty()) constraints += ",";
    constraints += clobber_constraints.str();
  }

  vector<llvm::Type *> arg_types;
  arg_types.reserve(arguments.size());
  for (auto arg : arguments) {
    arg_types.push_back(arg->getType());
  }

  auto llvm_function_type = llvm::FunctionType::get(
      LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type()),
      arg_types, false);

  auto inline_assembly =
      llvm::InlineAsm::get(llvm_function_type, expression->assembly->value,
                           constraints, true, false);

  return context->builder->CreateCall(inline_assembly, arguments);
}
