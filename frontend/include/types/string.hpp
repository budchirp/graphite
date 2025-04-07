#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>

#include "types/type.hpp"

class StringType : public Type {
 public:
  explicit StringType() = default;

  string get_name() const override { return "string"; }
  const type_info& get_type_info() const override { return typeid(StringType); }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override { return "StringType()"; };
};
