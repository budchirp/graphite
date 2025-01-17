#include "ast/statement/var.hpp"

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
  auto value = expression->codegen(context);
  if (!value) {
    Logger::error(
        "Failed to generate initializer for variable " + name->get_value(),
        LogTypes::Error::UNKNOWN, expression->get_position());
    return nullptr;
  }

  value = Codegen::cast_type(
      context, value,
      variable_type->get_type()->to_llvm(context->llvm_context));
  if (!value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  context->value_map.insert({name->get_value(), value});
  return value;
}

void VarStatement::analyze(const shared_ptr<ProgramContext> &context) {
  variable_type->analyze(context);
  expression->analyze(context);

  if (!Analyzer::compare(expression->get_type(), variable_type->get_type())) {
    Logger::error(
        "Type mismatch\nExpected `" + variable_type->get_type()->to_string() +
            "` Received `" + expression->get_type()->to_string() + "`",
        LogTypes::Error::TYPE_MISMATCH, variable_type->get_position());
    return;
  }
}

string VarStatement::to_string() const {
  ostringstream oss;
  oss << "var " << name->to_string() << ": " << type->to_string() << " = "
      << expression->to_string();
  return oss.str();
}

string VarStatement::to_string_tree() const {
  ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << expression->to_string_tree() << ", type: " << type->to_string_tree()
      << ")";
  return oss.str();
}
