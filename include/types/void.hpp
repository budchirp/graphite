#pragma once

#include "types/type.hpp"

class VoidType : public Type {
 public:
  explicit VoidType() = default;

  llvm::Type* to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::Type::getVoidTy(*context);
  };

  string get_name() const override { return "void"; }
  const type_info& get_type_info() const override { return typeid(VoidType); }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override { return "VoidType()"; };
};
