#pragma once

#include <llvm/IR/Value.h>

#include "codegen_llvm/context.hpp"

class LLVMCodegenUtils {
 public:
  static llvm::Value *cast_type(const shared_ptr<LLVMCodegenContext> &context,
                                llvm::Value *value, llvm::Type *expectedType,
                                bool strict = true);

  static llvm::Type *type_to_llvm_type(
      const shared_ptr<LLVMCodegenContext> &context,
      const shared_ptr<Type> &type);

  static llvm::Type *type_to_llvm_type(const LLVMCodegenContext *context,
                                       const shared_ptr<Type> &type);
};
