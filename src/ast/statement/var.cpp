#include "ast/statement/var.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include <memory>
#include <sstream>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *VarStatement::codegen(const shared_ptr<CodegenContext> &context) const {
  Value *value;
  if (expression) {
    value = expression->codegen(context);
  } else if (Analyzer::is_pointer(variable_type->get_type()).first) {
    value = context->get_env()->get_variable("nullptr")->value;
  } else {
    value = context->get_env()->get_variable("null")->value;
  }

  if (!value) {
    Logger::error(
        "Failed to generate initializer for variable " + name->get_identifier(),
        LogTypes::Error::UNKNOWN, expression->get_position());
    return nullptr;
  }

  auto type = variable_type->get_type()->to_llvm(context->llvm_context);
  value = Codegen::cast_type(context, value, type);
  if (!value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                  variable_type->get_position());
    return nullptr;
  }

  if (is_mutable) {
    auto ptr = context->builder->CreateAlloca(type, nullptr, "addr");
    context->builder->CreateStore(value, ptr);

    value = ptr;
  }

  context->get_env()->get_variable(name->get_identifier())->add_llvm_value(value);

  return value;
}

void VarStatement::analyze(const shared_ptr<ProgramContext> &context) {
  variable_type->analyze(context);
  if (expression) {
    expression->analyze(context);

    if (!Analyzer::compare(variable_type->get_type(), expression->get_type(),
                           true)) {
      Logger::error(
          "Type mismatch\nExpected `" + variable_type->get_type()->to_string() +
              "` Received `" + expression->get_type()->to_string() + "`",
          LogTypes::Error::TYPE_MISMATCH, variable_type->get_position());
      return;
    }
  }
}

string VarStatement::to_string() const {
  ostringstream oss;
  oss << "var " << name->to_string() << ": " << variable_type->to_string()
      << " = " << expression->to_string();
  return oss.str();
}

string VarStatement::to_string_tree() const {
  ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << expression->to_string_tree()
      << ", type: " << variable_type->to_string_tree() << ")";
  return oss.str();
}
