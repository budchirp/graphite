#pragma once

#include <string>

#include "types/type.hpp"

class FloatType : public Type {
 public:
  static string name;

  short size;

  explicit FloatType(const short size) : size(size) {};

  string get_name() const override { return name; }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override {
    return "FloatType(size: '" + ::to_string(size) + "')";
  };
};
