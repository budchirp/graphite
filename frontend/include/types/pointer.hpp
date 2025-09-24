#pragma once

#include <memory>
#include <string>

#include "types/type.hpp"

using namespace std;

class PointerType : public Type {
 public:
  static string name;

  shared_ptr<Type> pointee_type;

  bool is_mutable;

  explicit PointerType(const shared_ptr<Type> &pointee_type,
                       const bool &is_mutable)
      : pointee_type(pointee_type), is_mutable(is_mutable) {};

  string get_name() const override { return name; }

  string to_string() const override {
    return (is_mutable ? "mut *" : "*") +
           (pointee_type ? pointee_type->to_string() : "ptr");
  };
  string to_string_tree() const override {
    return "PointerType(pointee_type: " +
           (pointee_type ? pointee_type->to_string_tree() : "ptr") +
           ", is_mutable: '" + ::to_string(is_mutable) + "')";
  };
};
