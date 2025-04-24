#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <sys/types.h>

#include <memory>
#include <string>

#include "types/type.hpp"

using namespace std;

class ArrayType : public Type {
 public:
  shared_ptr<Type> child_type;
  uint size;

  explicit ArrayType(const shared_ptr<Type> &child_type, const uint &size)
      : child_type(child_type), size(size) {};

  string get_name() const override { return child_type->get_name(); }
  const type_info &get_type_info() const override { return typeid(ArrayType); }

  string to_string() const override {
    return child_type->to_string() + "[" + ::to_string(size) + "]";
  };
  string to_string_tree() const override {
    return "ArrayType(type: " + child_type->to_string_tree() + ", size: '" +
           ::to_string(size) + "')";
  };
};
