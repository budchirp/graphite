#include "codegen_llvm/expression/index.hpp"

#include "ast/expression/string.hpp"
#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

llvm::Value* IndexExpressionCodegen::codegen() const {
  auto index_value = LLVMCodegen::codegen(context, expression->index);
  if (!index_value) {
    Logger::error("Failed to generate low level code for index",
                  LogTypes::Error::INTERNAL, expression->index->get_position());
    return nullptr;
  }

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->identifier->value);

  if (auto array_type = TypeHelper::is_array(variable->type)) {
    auto index_ptr = context->builder->CreateGEP(
        LLVMCodegenUtils::type_to_llvm_type(context, array_type),
        context->get_variable(expression->identifier->value),
        {context->builder->getInt32(0), index_value}, "array_index");

    return context->builder->CreateLoad(
        LLVMCodegenUtils::type_to_llvm_type(context, array_type->child_type),
        index_ptr, "load");
  } else if (auto struct_type = TypeHelper::is_struct(variable->type)) {
    auto field =
        dynamic_pointer_cast<StringExpression>(expression->index)->value;

    shared_ptr<Type> field_type;

    size_t idx = 0;
    for (const auto& [type_field_name, type_field_type] : struct_type->fields) {
      if (type_field_name == field) {
        field_type = type_field_type;
        break;
      }

      idx++;
    }

    auto index_ptr = context->builder->CreateStructGEP(
        LLVMCodegenUtils::type_to_llvm_type(context, struct_type),
        context->get_variable(expression->identifier->value), idx, field);
    return context->builder->CreateLoad(
        LLVMCodegenUtils::type_to_llvm_type(context, field_type), index_ptr,
        "load");
  }

  return nullptr;
}
