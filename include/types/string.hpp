#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>

#include "types/type.hpp"

class StringType : public Type {
 private:
  string name = "string";


 public:
  explicit StringType() = default;

  llvm::Type* to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0);
  };

  string get_name() const override { return name; }
  const type_info& get_type_info() const override { return typeid(StringType); }

  string to_string() const override { return name; };
  string to_string_tree() const override { return "StringType()"; };
};
