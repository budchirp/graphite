#pragma once

#include <string>

#include "types/int.hpp"
#include "types/type.hpp"

class FloatType : public Type {
 public:
  short size;

  explicit FloatType(const short size) : size(size) {};

  string get_name() const override { return "f" + ::to_string(size); }
  const type_info& get_type_info() const override { return typeid(IntType); }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override {
    return "FloatType(size: '" + ::to_string(size) + "')";
  };
};
