#pragma once

#include "types/type.hpp"

class VoidType : public Type {
 public:
  explicit VoidType() {};

  llvm::Type *to_llvm(shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::Type::getVoidTy(*context);
  };

  string to_string() const override { return "void"; };
  string to_string_tree() const override { return "VoidType()"; };
};
