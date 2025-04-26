#pragma once

#include <memory>

#include "semantic/visibilty.hpp"
#include "types/type.hpp"

struct TypeSymbol {
  string name;

  SymbolVisibility::Value visibility;

  shared_ptr<Type> type;

  explicit TypeSymbol(const string &name,
                      const SymbolVisibility::Value &visibility,
                      const shared_ptr<Type> &type)
      : name(name), visibility(visibility), type(type) {}
};
