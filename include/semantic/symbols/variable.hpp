#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "semantic/linkage_type.hpp"
#include "semantic/visibilty.hpp"
#include "types/type.hpp"

struct VariableSymbol {
  string name;

  SymbolLinkageType::Value linkage;
  SymbolVisibility::Value visibility;

  shared_ptr<Type> type;

  llvm::Value *value;

  bool is_global;

  bool is_mutable;
  bool is_initialized;

  explicit VariableSymbol(const string &name,
                          const SymbolLinkageType::Value &linkage,
                          const SymbolVisibility::Value &visibility,
                          const shared_ptr<Type> &type, bool is_global,
                          bool is_mutable, bool is_initialized)
      : name(name),
        linkage(linkage),
        visibility(visibility),
        type(type),
        value(nullptr),
        is_global(is_global),
        is_mutable(is_mutable),
        is_initialized(is_initialized) {}

  void set_type(const shared_ptr<Type> &type) { this->type = type; }

  void add_llvm_value(llvm::Value *value) { this->value = value; }
};
