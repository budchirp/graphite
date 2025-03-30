#pragma once

#include <memory>
#include <unordered_map>

#include "semantic/scope/scope.hpp"
#include "semantic/symbols/function.hpp"
#include "types/type.hpp"

using namespace std;

class Env {
 private:
  unordered_map<string, shared_ptr<Type>> types;

  unordered_map<string, shared_ptr<FunctionSymbol>> functions;

  unordered_map<string, shared_ptr<Scope>> scopes;
  shared_ptr<Scope> current_scope;

  vector<string> includes;

 public:
  explicit Env();

  unordered_map<string, shared_ptr<FunctionSymbol>> get_functions() {
    return functions;
  }

  void add_type(const string &name, const shared_ptr<Type> &type);
  shared_ptr<Type> get_type(const string &name) const;

  void add_function(const string &name,
                    const shared_ptr<FunctionSymbol> &function);
  shared_ptr<FunctionSymbol> get_function(const string &name) const;

  void set_current_scope(const string &name);
  shared_ptr<Scope> get_current_scope();

  void add_scope(const string &name, const shared_ptr<Scope> &scope);
  shared_ptr<Scope> get_scope(const string &name);

  void add_include(const string &module_name);
  vector<string> get_includes();
};
