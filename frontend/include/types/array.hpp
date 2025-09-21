#pragma once

#include <memory>
#include <string>

#include "types/type.hpp"

using namespace std;

class ArrayType : public Type {
 public:
  static string name;

  shared_ptr<Type> child_type;
  
  int size;

  explicit ArrayType(const shared_ptr<Type> &child_type, const int &size)
      : child_type(child_type), size(size) {};

  string get_name() const override { return name; }

  string to_string() const override {
    return child_type->to_string() + "[" + (size >= 0 ? ::to_string(size) : "") + "]";
  };
  string to_string_tree() const override {
    return "ArrayType(type: " + child_type->to_string_tree() + ", size: '" +
           ::to_string(size) + "')";
  };
};
