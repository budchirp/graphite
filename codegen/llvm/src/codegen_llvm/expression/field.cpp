
#include "codegen_llvm/expression/field.hpp"

#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/options.hpp"
#include "codegen_llvm/utils.hpp"

llvm::Value* FieldExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value* FieldExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions>& options) const {
  size_t idx = 0;
  for (const auto& [type_field_name, type_field_type] :
       expression->get_struct_type()->fields) {
    if (type_field_name == expression->field->value) {
      break;
    }

    idx++;
  }

  auto struct_ptr =
      LLVMCodegen::codegen(context, expression->expression,
                           make_shared<CodegenOptions>(false, true));

  auto field_ptr = context->builder->CreateStructGEP(
      LLVMCodegenUtils::type_to_llvm_type(context,
                                          expression->get_struct_type()),
      struct_ptr, static_cast<unsigned>(idx));

  if (options && !options->load_variable) {
    return field_ptr;
  } else {
    return context->builder->CreateLoad(
        LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type()),
        field_ptr);
  }
}
