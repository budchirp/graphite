#pragma once

#include <llvm/IR/DerivedTypes.h>

#include "logger/logger.hpp"
#include "types/type.hpp"

class IntType : public Type {
 public:
  short size;
  bool is_unsigned;

  explicit IntType(const short size, const bool is_unsigned)
      : size(size), is_unsigned(is_unsigned) {};

  string get_name() const override { return "i" + ::to_string(size); }
  const type_info& get_type_info() const override { return typeid(IntType); }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override {
    return "IntType(size: '" + ::to_string(size) + "', is_unsigned: '" +
           ::to_string(is_unsigned) + "')";
  };
};
