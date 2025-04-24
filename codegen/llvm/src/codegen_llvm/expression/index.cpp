#include "codegen_llvm/expression/index.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *IndexExpressionCodegen::codegen() const {
  auto index_value = LLVMCodegen::codegen(context, expression->index);
  if (!index_value) {
    Logger::error("Failed to generate low level code for index",
                  LogTypes::Error::INTERNAL, expression->index->get_position());
    return nullptr;
  }

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->identifier->value);

  auto array_type = TypeHelper::is_array(variable->type);

  auto index_ptr = context->builder->CreateGEP(
      LLVMCodegenUtils::type_to_llvm_type(context, array_type),
      context->get_variable(expression->identifier->value),
      {context->builder->getInt32(0), index_value}, "array_index");

  return context->builder->CreateLoad(
      LLVMCodegenUtils::type_to_llvm_type(context, array_type->child_type),
      index_ptr, "load");
}
