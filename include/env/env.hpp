#pragma once

#include <memory>
#include <unordered_map>

#include "types/function.hpp"
#include "types/type.hpp"

using namespace std;

class Env {
 private:
  shared_ptr<Env> parent;

  unordered_map<string, shared_ptr<Type>> type_names;
  unordered_map<string, shared_ptr<Type>> variables;
  unordered_map<string, shared_ptr<FunctionType>> functions;

 public:
  explicit Env(const shared_ptr<Env> &parent) : parent(parent) {};

  void init();

  void set_type(const string &name, const shared_ptr<Type> &type);
  shared_ptr<Type> get_type(const string &name) const;

  void set_variable(const string &name, const shared_ptr<Type> &variable);
  shared_ptr<Type> get_variable(const string &name) const;

  void set_function(const string &name,
                    const shared_ptr<FunctionType> &function);
  shared_ptr<FunctionType> get_function(const string &name) const;

  shared_ptr<Env> get_parent() const { return parent; };
};
