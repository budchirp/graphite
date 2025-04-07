#include "codegen_llvm/expression/index.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *IndexExpressionCodegen::codegen() const {
  auto name_value = LLVMCodegen::codegen(context, expression->variable);
  if (!name_value) {
    Logger::error(
        "Failed to generate low level code for variable reference in index "
        "expression",
        LogTypes::Error::INTERNAL, expression->variable->get_position());
    return nullptr;
  }

  auto index_value = LLVMCodegen::codegen(context, expression->index);
  if (!index_value) {
    Logger::error("Failed to generate low level code for index",
                  LogTypes::Error::INTERNAL, expression->index->get_position());
    return nullptr;
  }

  auto array_type = TypeHelper::is_array(expression->variable->get_type());

  auto index_ptr = context->builder->CreateGEP(
      LLVMCodegenUtils::type_to_llvm_type(context, array_type), name_value,
      {context->builder->getInt32(0), index_value}, "array_index");
  auto value = context->builder->CreateLoad(
      LLVMCodegenUtils::type_to_llvm_type(context, array_type->child_type),
      index_ptr, "load");

  return value;
}
