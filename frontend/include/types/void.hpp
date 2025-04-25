#pragma once

#include "types/type.hpp"

class VoidType : public Type {
 public:
  static string name;

  explicit VoidType() = default;

  string get_name() const override { return name; }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override { return "VoidType()"; };
};
