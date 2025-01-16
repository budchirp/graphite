#pragma once

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "types/type.hpp"

using namespace std;

class PointerType : public Type {
 public:
  shared_ptr<Type> type;

  explicit PointerType(const shared_ptr<Type> &type) : type(type) {};

  llvm::PointerType *to_llvm(const shared_ptr<llvm::LLVMContext> context) const override {
    return llvm::PointerType::get(type->to_llvm(context), 0);
  };

  string to_string() const override { return "*" + type->to_string(); };
  string to_string_tree() const override {
    return "PointerType(type: " + type->to_string_tree() + ")";
  };
};
