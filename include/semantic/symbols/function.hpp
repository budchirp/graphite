#pragma once

#include "types/function.hpp"

struct FunctionSymbol {
  string name;

  shared_ptr<FunctionType> type;

  llvm::Function *function;

  explicit FunctionSymbol(const string &name, const shared_ptr<FunctionType> &type)
      : name(name), type(type), function(nullptr) {}

  void add_llvm_value(llvm::Function *function) { this->function = function; }
};
