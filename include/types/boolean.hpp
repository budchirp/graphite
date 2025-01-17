#pragma once

#include <llvm/IR/DerivedTypes.h>

#include "types/type.hpp"

class BooleanType : public Type {
 public:
  explicit BooleanType() = default;

  llvm::IntegerType* to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::Type::getInt1Ty(*context);
  }

  const type_info& get_type_info() const override {
    return typeid(BooleanType);
  }

  string to_string() const override { return "boolean"; };
  string to_string_tree() const override { return "BooleanType()"; };
};
