#include "ast/statement/var.hpp"

#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/symbols/variable.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *VarStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(name->get_identifier());

  auto llvm_type = type->to_llvm(context->llvm_context);

  llvm::Value *value = llvm::Constant::getNullValue(llvm_type);
  if (variable->is_initialized) {
    value = expression->codegen(context);
    if (!value) {
      Logger::error("Failed to generate initializer for variable " +
                        name->get_identifier(),
                    LogTypes::Error::UNKNOWN, expression->get_position());
      return nullptr;
    }
  }

  value = Codegen::cast_type(context, value, llvm_type);
  if (!value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                  variable_type->get_position());
    return nullptr;
  }

  if (variable->is_global) {
    llvm::Constant *initializer = nullptr;
    if (auto *const_value = dyn_cast<llvm::Constant>(value)) {
      initializer = const_value;
    } else {
      initializer = llvm::Constant::getNullValue(llvm_type);
      Logger::warn(
          "Global variable '" + name->get_identifier() +
              "' requires a constant initializer; using default zero value",
          LogTypes::Warn::SUGGESTION, variable_type->get_position());
    }

    auto global_variable = new llvm::GlobalVariable(
        *context->module, llvm_type, !variable->is_mutable,
        llvm::GlobalValue::ExternalLinkage, initializer,
        name->get_identifier());

    value = global_variable;
  }

  if (variable->is_mutable && !variable->is_global) {
    auto ptr = context->builder->CreateAlloca(llvm_type, nullptr, "addr");
    context->builder->CreateStore(value, ptr);

    value = ptr;
  }

  scope->get_variable(name->get_identifier())->add_llvm_value(value);

  return value;
}

void VarStatement::validate(const shared_ptr<ProgramContext> &context) {
  if (variable_type) variable_type->validate(context);
  if (expression) {
    expression->validate(context);

    if (variable_type) {
      if (!TypeHelper::compare(variable_type->get_type(),
                               expression->get_type(), true)) {
        Logger::error(
            "Type mismatch\nExpected `" +
                variable_type->get_type()->to_string() + "` Received `" +
                expression->get_type()->to_string() + "`",
            LogTypes::Error::TYPE_MISMATCH, variable_type->get_position());
        return;
      }
    }
  }
}

void VarStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  if (variable_type) variable_type->resolve_types(context);
  if (expression) expression->resolve_types(context);

  if (!expression) {
    type = variable_type->get_type();
  } else {
    type = expression->get_type();
  }

  auto scope = context->get_env()->get_current_scope();
  auto is_global = scope->get_name() == "global";

  scope->add_variable(
      name->get_identifier(),
      make_shared<VariableSymbol>(name->get_identifier(),
                                  SymbolLinkageType::Value::Internal, visibility,
                                  type, is_global, is_mutable, is_initialized));
}

string VarStatement::to_string() const {
  ostringstream oss;
  oss << "var " << name->to_string() << ": " << variable_type->to_string()
      << expression
      ? (" = " + expression->to_string())
      : "";
  return oss.str();
}

string VarStatement::to_string_tree() const {
  ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << (expression ? expression->to_string_tree() : "empty") << ", type: "
      << (variable_type ? variable_type->to_string_tree() : "empty") << ")";
  return oss.str();
}
