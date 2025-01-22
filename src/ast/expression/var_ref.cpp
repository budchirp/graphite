
#include "ast/expression/var_ref.hpp"

#include <alloca.h>
#include <llvm/IR/Value.h>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *VarRefExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto it = context->value_map.find(identifier->get_value());
  if (it == context->value_map.end()) {
    Logger::error("Undefined variable `" + identifier->get_value() + "`",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  auto value = it->second;
  if (Analyzer::is_pointer(type).first) return value;

  return value->getType()->isPointerTy()
             ? context->builder->CreateLoad(
                   type->to_llvm(context->llvm_context), value, "load")
             : value;
}

void VarRefExpression::analyze(const shared_ptr<ProgramContext> &context) {
  if (!context->get_env()->get_variable(identifier->get_value())) {
    Logger::error("Undefined variable `" + identifier->get_value() + "`",
                  LogTypes::Error::UNDEFINED, &position);
  }
}

string VarRefExpression::to_string() const { return identifier->to_string(); }

string VarRefExpression::to_string_tree() const {
  return "VarRefExpression(type: " + type->to_string_tree() +
         ", identifier: '" + identifier->get_value() + "')";
}
