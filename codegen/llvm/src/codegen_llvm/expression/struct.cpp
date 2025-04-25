#include "codegen_llvm/expression/struct.hpp"

#include <llvm/IR/Constants.h>

#include <cstddef>
#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"
#include "types/struct.hpp"

llvm::Value* StructExpressionCodegen::codegen() const {
  auto struct_type = dynamic_pointer_cast<StructType>(
      context->get_env()->get_type(expression->name->value));

  auto llvm_struct_type =
      LLVMCodegenUtils::type_to_llvm_type(context, struct_type);

  auto struct_ptr =
      context->builder->CreateAlloca(llvm_struct_type, nullptr, "struct");

  for (const auto& [field_name, field_value] : expression->fields) {
    auto llvm_value = LLVMCodegen::codegen(context, field_value);
    if (!llvm_value) {
      Logger::error("Failed to generate low level code for struct value for `" +
                        field_name->value + "`",
                    LogTypes::Error::INTERNAL, field_value->get_position());
      return nullptr;
    }

    shared_ptr<Type> field_type;

    size_t idx = 0;
    for (const auto& [type_field_name, type_field_type] : struct_type->fields) {
      if (type_field_name == field_name->value) {
        field_type = type_field_type;
        break;
      }

      idx++;
    }

    llvm_value = LLVMCodegenUtils::cast_type(
        context, llvm_value,
        LLVMCodegenUtils::type_to_llvm_type(context, field_type));
    if (!llvm_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    field_value->get_position());
      return nullptr;
    }

    auto index_ptr = context->builder->CreateStructGEP(
        llvm_struct_type, struct_ptr, idx, field_name->value);
    context->builder->CreateStore(llvm_value, index_ptr);
  }

  return struct_ptr;
}
