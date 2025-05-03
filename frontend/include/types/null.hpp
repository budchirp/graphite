#pragma once

#include <memory>

#include "types/type.hpp"

using namespace std;

class NullType : public Type {
 public:
  static string name;

  shared_ptr<Type> child_type;

  explicit NullType(const shared_ptr<Type> &child_type)
      : child_type(child_type) {};

  string get_name() const override { return name; }

  string to_string() const override {
    return (child_type ? child_type->to_string() : "nullptr") + "?";
  };
  string to_string_tree() const override {
    return "NullType(child_type: " +
           (child_type ? child_type->to_string_tree() : "nullptr") + ")";
  };
};
