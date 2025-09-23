#include "codegen_llvm/expression/assembly.hpp"

#include <sstream>
#include <vector>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"

llvm::Value *AssemblyExpressionCodegen::codegen() const {
  vector<llvm::Value *> arguments;
  auto scope = context->get_env()->get_current_scope();

  ostringstream output_constraints;
  for (const auto &[constraint, variable] : expression->output) {
    auto variable_symbol = scope->get_variable(variable->value);
    auto address = context->get_variable(variable_symbol);
    arguments.push_back(address);

    if (output_constraints.tellp() > 0) {
      output_constraints << ",";
    }
    output_constraints << constraint->value;
  }

  ostringstream input_constraints;
  for (const auto &[constraint, variable] : expression->input) {
    auto variable_symbol = scope->get_variable(variable->value);
    auto value = context->get_variable_value(variable_symbol);
    arguments.push_back(value);

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

  // Create function type: return type based on expression type, arguments based
  // on input/output
  auto return_type =
      LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type());
  vector<llvm::Type *> arg_types;
  for (auto arg : arguments) {
    arg_types.push_back(arg->getType());
  }
  auto function_type = llvm::FunctionType::get(return_type, arg_types, false);

  auto inline_assembly = llvm::InlineAsm::get(
      function_type, expression->assembly->value, constraints, true, false);

  return context->builder->CreateCall(inline_assembly, arguments);
}
