#include "env/env.hpp"

#include <memory>

#include "logger/logger.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/null.hpp"
#include "types/pointer.hpp"
#include "types/string.hpp"
#include "types/type.hpp"
#include "types/void.hpp"

void Env::init() {
  type_names.emplace("string", make_shared<StringType>());
  type_names.emplace("i8", make_shared<IntType>(8, false));
  type_names.emplace("i16", make_shared<IntType>(16, false));
  type_names.emplace("i32", make_shared<IntType>(32, false));
  type_names.emplace("i64", make_shared<IntType>(64, false));
  type_names.emplace("u8", make_shared<IntType>(8, true));
  type_names.emplace("u16", make_shared<IntType>(16, true));
  type_names.emplace("u32", make_shared<IntType>(32, true));
  type_names.emplace("u64", make_shared<IntType>(64, true));
  type_names.emplace("f8", make_shared<FloatType>(8));
  type_names.emplace("f16", make_shared<FloatType>(16));
  type_names.emplace("f32", make_shared<FloatType>(32));
  type_names.emplace("f64", make_shared<FloatType>(64));
  type_names.emplace("boolean", make_shared<BooleanType>());
  type_names.emplace("void", make_shared<VoidType>());
  type_names.emplace("null", make_shared<NullType>(nullptr));

  variables.emplace(
      "null", make_shared<EnvVariable>(make_shared<NullType>(nullptr), false));
  variables.emplace(
      "nullptr",
      make_shared<EnvVariable>(
          make_shared<PointerType>(make_shared<NullType>(nullptr)), false));
}

void Env::add_type(const string &name, const shared_ptr<Type> &type) {
  type_names.insert_or_assign(name, type);
}

shared_ptr<Type> Env::get_type(const string &name) const {
  auto it = type_names.find(name);
  if (it != type_names.end()) {
    return it->second;
  }

  if (parent) {
    return parent->get_type(name);
  }

  Logger::warn("Accessed to an undefined type `" + name + "`");
  return nullptr;
}

void Env::add_variable(const string &name,
                       const shared_ptr<EnvVariable> &variable) {
  variables.insert_or_assign(name, variable);
}

shared_ptr<EnvVariable> Env::get_variable(const string &name) const {
  auto it = variables.find(name);
  if (it != variables.end()) {
    return it->second;
  }

  if (parent) {
    return parent->get_variable(name);
  }

  Logger::warn("Accessed to an undefined symbol `" + name + "`");
  return nullptr;
}

void Env::add_function(const string &name,
                       const shared_ptr<EnvFunction> &function) {
  functions.insert_or_assign(name, function);
}

shared_ptr<EnvFunction> Env::get_function(const string &name) const {
  auto it = functions.find(name);
  if (it != functions.end()) {
    return it->second;
  }

  if (parent) {
    return parent->get_function(name);
  }

  Logger::warn("Accessed to an undefined symbol `" + name + "`");
  return nullptr;
}
