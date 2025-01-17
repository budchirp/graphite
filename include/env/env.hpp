#pragma once

#include <memory>
#include <unordered_map>

#include "types/type.hpp"

using namespace std;

class Env {
 private:
  shared_ptr<Env> parent;

  unordered_map<string, shared_ptr<Type>> type_map;
  unordered_map<string, shared_ptr<Type>> symbol_map;

 public:
  explicit Env(const shared_ptr<Env> &parent) : parent(parent) {};
  ~Env() {
    type_map.clear();
    symbol_map.clear();
  }

  void init();

  void set_type(string name, shared_ptr<Type> type);
  shared_ptr<Type> get_type(const string &name) const;

  void set_symbol(string name, shared_ptr<Type> symbol);
  shared_ptr<Type> get_symbol(const string &name) const;

  shared_ptr<Env> get_parent() const { return parent; };
};
