#pragma once

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "types/type.hpp"

using namespace std;

class PointerType : public Type {
 public:
  shared_ptr<Type> pointee_type;

  explicit PointerType(const shared_ptr<Type> &pointee_type)
      : pointee_type(pointee_type) {};

  string get_name() const override { return pointee_type->get_name(); }
  const type_info &get_type_info() const override {
    return typeid(PointerType);
  }

  string to_string() const override { return "*" + pointee_type->to_string(); };
  string to_string_tree() const override {
    return "PointerType(pointee_type: " + pointee_type->to_string_tree() + ")";
  };
};
