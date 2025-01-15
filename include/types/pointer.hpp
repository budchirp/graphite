#pragma once

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "types/type.hpp"

using namespace std;

class PointerType : public Type {
 public:
  shared_ptr<Type> type;

  PointerType(shared_ptr<Type> type) : type(type) {};

  llvm::PointerType *to_llvm(shared_ptr<llvm::LLVMContext> context) override {
    return llvm::PointerType::get(type->to_llvm(context), 0);
  };

  string to_string() const override { return "*" + type->to_string(); };
  string to_string_tree() const override {
    return "PointerType(type: " + type->to_string_tree() + ")";
  };
};
