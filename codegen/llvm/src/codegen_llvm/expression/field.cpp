
#include "codegen_llvm/expression/field.hpp"

#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"

llvm::Value* FieldExpressionCodegen::codegen() const {
  size_t idx = 0;
  for (const auto& [type_field_name, type_field_type] : expression->get_struct_type()->fields) {
    if (type_field_name == expression->field->value) {
      break;
    }

    idx++;
  }

  auto field_ptr = context->builder->CreateStructGEP(
      LLVMCodegenUtils::type_to_llvm_type(context, expression->get_struct_type()),
      context->get_variable(expression->identifier->value), idx,
      expression->field->value);
  return context->builder->CreateLoad(
      LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type()), field_ptr,
      "load");
}
