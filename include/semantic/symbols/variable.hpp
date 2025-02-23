#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "types/type.hpp"

struct VariableSymbol {
  string name;

  shared_ptr<Type> type;

  llvm::Value *value;

  bool is_mutable;
  bool is_initialized;

  explicit VariableSymbol(const string &name, const shared_ptr<Type> &type,
                          bool is_mutable, bool is_initialized)
      : name(name),
        type(type),
        value(nullptr),
        is_mutable(is_mutable),
        is_initialized(is_initialized) {}

  void set_type(const shared_ptr<Type> &type) { this->type = type; }

  void add_llvm_value(llvm::Value *value) { this->value = value; }
};
