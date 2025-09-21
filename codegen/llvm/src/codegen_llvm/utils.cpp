#include "codegen_llvm/utils.hpp"

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "logger/logger.hpp"
#include "types/array.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/null.hpp"
#include "types/pointer.hpp"
#include "types/string.hpp"
#include "types/struct.hpp"
#include "types/void.hpp"

llvm::Value *LLVMCodegenUtils::cast_type(
    const shared_ptr<LLVMCodegenContext> &context, llvm::Value *value,
    llvm::Type *expectedType, bool strict) {
  if (value->getType()->isPointerTy() || expectedType->isPointerTy())
    return value;

  if (value->getType() == expectedType) {
    return value;
  }

  if (value->getType()->isIntegerTy() && expectedType->isIntegerTy()) {
    if (value->getType()->getIntegerBitWidth() >
        expectedType->getIntegerBitWidth()) {
      return context->builder->CreateTrunc(value, expectedType);
    }

    return context->builder->CreateSExt(value, expectedType);
  }

  if (value->getType()->isFloatingPointTy() &&
      expectedType->isFloatingPointTy()) {
    if (value->getType()->getPrimitiveSizeInBits() >
        expectedType->getPrimitiveSizeInBits()) {
      return context->builder->CreateFPTrunc(value, expectedType);
    }

    return context->builder->CreateFPExt(value, expectedType);
  }

  if (value->getType()->isIntegerTy() && expectedType->isFloatingPointTy()) {
    return context->builder->CreateSIToFP(value, expectedType);
  }

  if (value->getType()->isFloatingPointTy() && expectedType->isIntegerTy()) {
    return context->builder->CreateFPToSI(value, expectedType);
  }

  return strict ? nullptr : value;
}

llvm::Type *LLVMCodegenUtils::type_to_llvm_type(
    const shared_ptr<LLVMCodegenContext> &context,
    const shared_ptr<Type> &type) {
  return type_to_llvm_type(context.get(), type);
}

llvm::Type *LLVMCodegenUtils::type_to_llvm_type(
    const LLVMCodegenContext *context, const shared_ptr<Type> &type) {
  if (auto array_type = dynamic_pointer_cast<ArrayType>(type)) {
    if (array_type->size >= 0) {
      return llvm::ArrayType::get(
          type_to_llvm_type(context, array_type->child_type), array_type->size);
    } else {
      return llvm::PointerType::get(
          type_to_llvm_type(context, array_type->child_type), 0);
    }
  }

  if (auto boolean_type = dynamic_pointer_cast<BooleanType>(type)) {
    return llvm::Type::getInt1Ty(*context->llvm_context);
  }

  if (auto float_type = dynamic_pointer_cast<FloatType>(type)) {
    switch (float_type->size) {
      case 16:
        return llvm::Type::getHalfTy(*context->llvm_context);
      case 32:
        return llvm::Type::getFloatTy(*context->llvm_context);
      case 64:
        return llvm::Type::getDoubleTy(*context->llvm_context);
      case 128:
        return llvm::Type::getFP128Ty(*context->llvm_context);

      default:
        Logger::error("Unsupported floating-point size: " +
                      std::to_string(float_type->size));
        return nullptr;
    }
  }

  if (auto function_type = dynamic_pointer_cast<FunctionType>(type)) {
    vector<llvm::Type *> parameters;
    parameters.reserve(function_type->parameters.size());
    for (const auto &[_, parameter_type] : function_type->parameters) {
      parameters.push_back(type_to_llvm_type(context, parameter_type));
    }

    return llvm::FunctionType::get(LLVMCodegenUtils::type_to_llvm_type(
                                       context, function_type->return_type),
                                   parameters, false);
  }

  if (auto int_type = dynamic_pointer_cast<IntType>(type)) {
    switch (int_type->size) {
      case 8:
        return llvm::Type::getInt8Ty(*context->llvm_context);
      case 16:
        return llvm::Type::getInt16Ty(*context->llvm_context);
      case 32:
        return llvm::Type::getInt32Ty(*context->llvm_context);
      case 64:
        return llvm::Type::getInt64Ty(*context->llvm_context);

      default:
        Logger::error("Unsupported int size: " +
                      std::to_string(int_type->size));
        return nullptr;
    }
  }

  if (auto null_type = dynamic_pointer_cast<NullType>(type)) {
    return null_type->child_type
               ? type_to_llvm_type(context, null_type->child_type)
               : llvm::Type::getVoidTy(*context->llvm_context);
  }

  if (auto pointer_type = dynamic_pointer_cast<PointerType>(type)) {
    return llvm::PointerType::get(*context->llvm_context, 0);
  }

  if (auto string_type = dynamic_pointer_cast<StringType>(type)) {
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context->llvm_context),
                                  0);
  }

  if (auto void_type = dynamic_pointer_cast<VoidType>(type)) {
    return llvm::Type::getVoidTy(*context->llvm_context);
  }

  if (auto struct_type = dynamic_pointer_cast<StructType>(type)) {
    vector<llvm::Type *> fields;
    fields.reserve(struct_type->fields.size());
    for (const auto &[_, field_type] : struct_type->fields) {
      fields.push_back(type_to_llvm_type(context, field_type));
    }

    return llvm::StructType::get(*context->llvm_context, fields);
  }

  Logger::error("Unknown type");
  return nullptr;
}
