#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"
#include "ast/statement/var.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *VarStatement::codegen() const {
  Value *value = expression->codegen();
  if (!value) {
    Logger::error(
        "Failed to generate initializer for variable " + name->get_value(),
        LogTypes::Error::UNKNOWN, expression->get_position());
    return nullptr;
  }

  value = Codegen::cast_type(value, type->get_type()->to_llvm(context));
  if (!value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  named_values.insert({name->get_value(), value});
  return value;
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
