#include <iostream>

#include "logger/logger.hpp"
#include "env/env.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/string.hpp"
#include "types/type.hpp"
#include "types/void.hpp"

void Env::init() {
  type_map.emplace("string", make_shared<StringType>());
  type_map.emplace("i8", make_shared<IntType>(8, false));
  type_map.emplace("i16", make_shared<IntType>(16, false));
  type_map.emplace("i32", make_shared<IntType>(32, false));
  type_map.emplace("i64", make_shared<IntType>(64, false));
  type_map.emplace("u8", make_shared<IntType>(8, true));
  type_map.emplace("u16", make_shared<IntType>(16, true));
  type_map.emplace("u32", make_shared<IntType>(32, true));
  type_map.emplace("u64", make_shared<IntType>(64, true));
  type_map.emplace("f8", make_shared<FloatType>(8));
  type_map.emplace("f16", make_shared<FloatType>(16));
  type_map.emplace("f32", make_shared<FloatType>(32));
  type_map.emplace("f64", make_shared<FloatType>(64));
  type_map.emplace("boolean", make_shared<BooleanType>());
  type_map.emplace("void", make_shared<VoidType>());
}

void Env::set_type(string name, shared_ptr<Type> type) {
  cout << "set: " << name << endl;

  type_map.insert_or_assign(name, type);
}

shared_ptr<Type> Env::get_type(const string &name) const {
  auto it = type_map.find(name);
  if (it != type_map.end()) {
    return it->second;
  }

  if (parent) {
    return parent->get_type(name);
  }

  Logger::warn("Accessed to an undefined type `" + name + "`");
  return nullptr;
}

void Env::set_symbol(string name, shared_ptr<Type> symbol) {
  cout << "set: " << name << endl;

  symbol_map.insert_or_assign(name, symbol);
}

shared_ptr<Type> Env::get_symbol(const string &name) const {
  auto it = symbol_map.find(name);
  if (it != symbol_map.end()) {
    return it->second;
  }

  if (parent) {
    return parent->get_symbol(name);
  }

  Logger::warn("Accessed to an undefined symbol `" + name + "`");
  return nullptr;
}
