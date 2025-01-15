#pragma once

#include <iostream>
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
  Env(shared_ptr<Env> parent) : parent(parent) {};
  ~Env() {
    cout << endl << "Type map:" << endl;
    for (const auto &pair : type_map) {
      cout << pair.first << ": " << pair.second->to_string_tree() << std::endl;
    }

    cout << endl << "Symbol map:" << endl;
    for (const auto &pair : symbol_map) {
      cout << pair.first << ": " << pair.second->to_string_tree() << std::endl;
    }
  };

  void init();

  void set_type(string name, shared_ptr<Type> type);
  shared_ptr<Type> get_type(const string &name);

  void set_symbol(string name, shared_ptr<Type> symbol);
  shared_ptr<Type> get_symbol(const string &name);

  shared_ptr<Env> get_parent() { return parent; };
};
