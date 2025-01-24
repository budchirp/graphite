
#pragma once

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "types/type.hpp"

using namespace std;

class NullType : public Type {
 public:
  shared_ptr<Type> child_type;

  explicit NullType(const shared_ptr<Type> &child_type)
      : child_type(child_type) {};

  llvm::Type *to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    if (!child_type) {
      return llvm::Type::getInt1Ty(*context);
    }

    return child_type->to_llvm(context);
  };

  string get_name() const override {
    return child_type ? child_type->get_name() : "null";
  }
  const type_info &get_type_info() const override { return typeid(NullType); }

  string to_string() const override {
    return child_type ? (child_type->to_string() + "?") : "null";
  };
  string to_string_tree() const override {
    return "NullType(type: " +
           (child_type ? child_type->to_string_tree() : "null") + ")";
  };
};
