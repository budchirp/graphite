#pragma once

#include <memory>
#include <string>

#include "semantic/symbols/variable.hpp"

using namespace std;

class Scope {
 private:
  string name;

  shared_ptr<Scope> parent;

  unordered_map<string, shared_ptr<VariableSymbol>> variables;

 public:
  explicit Scope(const string &name, const shared_ptr<Scope> &parent) : name(name), parent(parent) {}

  string get_name() const {
    return name;
  }

  shared_ptr<Scope> get_parent() const {
    return parent;
  }

  void add_variable(const string &name,
                    const shared_ptr<VariableSymbol> &variable);
  shared_ptr<VariableSymbol> get_variable(const string &name) const;
};
