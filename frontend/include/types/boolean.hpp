#pragma once

#include <llvm/IR/DerivedTypes.h>

#include "types/type.hpp"

class BooleanType : public Type {
 public:
  explicit BooleanType() = default;

  string get_name() const override { return "boolean"; }
  const type_info& get_type_info() const override {
    return typeid(BooleanType);
  }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override { return "BooleanType()"; };
};
