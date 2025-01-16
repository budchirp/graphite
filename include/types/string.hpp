#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>

#include "types/type.hpp"

class StringType : public Type {
 public:
  explicit StringType() {};

  llvm::Type *to_llvm(shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0);
  };

  string to_string() const override { return "string"; };
  string to_string_tree() const override { return "StringType()"; };
};
