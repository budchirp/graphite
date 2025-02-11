
#include "ast/expression/var_ref.hpp"

#include <alloca.h>
#include <llvm/IR/Value.h>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *VarRefExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto variable = context->get_env()->get_current_scope()->get_variable(name);
  if (!variable || !variable->value) {
    Logger::error("Undefined variable `" + name + "`",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  auto value = variable->value;
  return (!variable->is_initialized || variable->is_mutable)
             ? context->builder->CreateLoad(
                   type->to_llvm(context->llvm_context), value, "load")
             : value;
}

void VarRefExpression::validate(const shared_ptr<ProgramContext> &context) {
  auto variable = context->get_env()->get_current_scope()->get_variable(name);
  if (!variable || !variable->type) {
    Logger::error("Undefined variable `" + name + "`",
                  LogTypes::Error::UNDEFINED, &position);
  }
}

void VarRefExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  auto variable = context->get_env()->get_current_scope()->get_variable(name);
  if (!variable || !variable->type) {
    Logger::error("Undefined variable `" + name + "`",
                  LogTypes::Error::UNDEFINED, &position);
    return;
  }

  type = variable->type;
}

string VarRefExpression::to_string() const { return name; }

string VarRefExpression::to_string_tree() const {
  return "VarRefExpression(type: " + type->to_string_tree() + ", name: '" +
         name + "')";
}
