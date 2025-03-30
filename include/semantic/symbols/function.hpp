#pragma once

#include "semantic/linkage_type.hpp"
#include "semantic/visibilty.hpp"
#include "types/function.hpp"

struct FunctionSymbol {
  string name;

  SymbolLinkageType::Value linkage;
  SymbolVisibility::Value visibility;

  shared_ptr<FunctionType> type;

  llvm::Function *function;

  explicit FunctionSymbol(const string &name,
                          const SymbolLinkageType::Value &linkage,
                          const SymbolVisibility::Value &visibility,
                          const shared_ptr<FunctionType> &type)
      : name(name),
        linkage(linkage),
        visibility(visibility),
        type(type),
        function(nullptr) {}

  void add_llvm_value(llvm::Function *function) { this->function = function; }
};
