#pragma once

#include <llvm/IR/DerivedTypes.h>
#include "types/type.hpp"

class BooleanType : public Type {
 public:
  BooleanType() {};

  llvm::IntegerType* to_llvm(std::shared_ptr<llvm::LLVMContext> context) override {
    return llvm::Type::getInt1Ty(*context);
  }

  string to_string() const override { return "boolean"; };
  string to_string_tree() const override { return "BooleanType()"; };
};
