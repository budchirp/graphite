#pragma once

#include "types/type.hpp"

class BooleanType : public Type {
 public:
  static string name;

  explicit BooleanType() = default;

  string get_name() const override { return name; }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override { return "BooleanType()"; };
};
