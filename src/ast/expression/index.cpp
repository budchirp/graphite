
#include "ast/expression/index.hpp"

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/array.hpp"

llvm::Value *IndexExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto name_value = variable->codegen(context);
  if (!name_value) {
    Logger::error(
        "Failed to generate low level code for variable reference in index "
        "expression",
        LogTypes::Error::INTERNAL, variable->get_position());
    return nullptr;
  }

  auto index_value = index->codegen(context);
  if (!index_value) {
    Logger::error("Failed to generate low level code for index",
                  LogTypes::Error::INTERNAL, index->get_position());
    return nullptr;
  }

  auto array_type = dynamic_pointer_cast<ArrayType>(variable->get_type());

  auto index_ptr = context->builder->CreateGEP(
      array_type->to_llvm(context->llvm_context), name_value,
      {context->builder->getInt32(0), index_value}, "array_index");
  auto value = context->builder->CreateLoad(
      array_type->child_type->to_llvm(context->llvm_context), index_ptr,
      "load");

  return value;
}

void IndexExpression::validate(const shared_ptr<ProgramContext> &context) {
  variable->validate(context);
  index->validate(context);

  if (!TypeHelper::is_array(variable->get_type())) {
    Logger::error("Array expected", LogTypes::Error::TYPE_MISMATCH,
                  variable->get_position());
    return;
  }

  if (!TypeHelper::is_int(index->get_type())) {
    Logger::error("Expected integer as index", LogTypes::Error::TYPE_MISMATCH,
                  index->get_position());
    return;
  }
}

void IndexExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  variable->resolve_types(context);
  index->resolve_types(context);

  if (auto array_type = TypeHelper::is_array(variable->get_type())) {
    type = array_type->child_type;
  } else {
    Logger::error("Array expected");
  }
}

string IndexExpression::to_string() const {
  return variable->to_string() + "[" + index->to_string() + "]";
}

string IndexExpression::to_string_tree() const {
  return "IndexExpression(type: " + type->to_string_tree() +
         ", variable: " + variable->to_string_tree() +
         ", index: " + index->to_string_tree() + ")";
}
