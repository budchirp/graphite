#include "semantic/scope/scope.hpp"

#include "logger/logger.hpp"
#include "semantic/symbols/variable.hpp"

void Scope::add_variable(const string &name,
                         const shared_ptr<VariableSymbol> &variable) {
  variables.insert_or_assign(name, variable);
}

shared_ptr<VariableSymbol> Scope::get_variable(const string &name) const {
  auto it = variables.find(name);
  if (it != variables.end() && it->second) {
    return it->second;
  }

  if (parent) {
    if (auto var = parent->get_variable(name)) {
      return var;
    }
  }

  Logger::warn("Accessed to an undefined variable `" + name + "`");
  return nullptr;
}
