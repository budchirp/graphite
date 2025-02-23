#include "semantic/env/env.hpp"

#include <memory>

#include "logger/logger.hpp"
#include "semantic/symbols/function.hpp"
#include "semantic/symbols/variable.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/null.hpp"
#include "types/string.hpp"
#include "types/type.hpp"
#include "types/void.hpp"

Env::Env() {
  types.emplace("string", make_shared<StringType>());
  types.emplace("i8", make_shared<IntType>(8, false));
  types.emplace("i16", make_shared<IntType>(16, false));
  types.emplace("i32", make_shared<IntType>(32, false));
  types.emplace("i64", make_shared<IntType>(64, false));
  types.emplace("u8", make_shared<IntType>(8, true));
  types.emplace("u16", make_shared<IntType>(16, true));
  types.emplace("u32", make_shared<IntType>(32, true));
  types.emplace("u64", make_shared<IntType>(64, true));
  types.emplace("f8", make_shared<FloatType>(8));
  types.emplace("f16", make_shared<FloatType>(16));
  types.emplace("f32", make_shared<FloatType>(32));
  types.emplace("f64", make_shared<FloatType>(64));
  types.emplace("boolean", make_shared<BooleanType>());
  types.emplace("void", make_shared<VoidType>());

  auto global_scope = make_shared<Scope>("global", nullptr);
  add_scope("global", global_scope);
  set_current_scope("global");

  current_scope->add_variable(
      "null",
      make_shared<VariableSymbol>("null", make_shared<NullType>(nullptr), false, true));
}

void Env::add_type(const string &name, const shared_ptr<Type> &type) {
  types.insert_or_assign(name, type);
}

shared_ptr<Type> Env::get_type(const string &name) const {
  auto it = types.find(name);
  if (it != types.end()) {
    return it->second;
  }

  Logger::warn("Accessed to an undefined type `" + name + "`");
  return nullptr;
}

void Env::add_function(const string &name,
                       const shared_ptr<FunctionSymbol> &function) {
  functions.insert_or_assign(name, function);
}

shared_ptr<FunctionSymbol> Env::get_function(const string &name) const {
  auto it = functions.find(name);
  if (it != functions.end()) {
    return it->second;
  }

  Logger::warn("Accessed to an undefined symbol `" + name + "`");
  return nullptr;
}

void Env::set_current_scope(const string &name) {
  current_scope = get_scope(name);
}
shared_ptr<Scope> Env::get_current_scope() { return current_scope; }

void Env::add_scope(const string &name, const shared_ptr<Scope> &scope) {
  scopes.emplace(name, scope);
}
shared_ptr<Scope> Env::get_scope(const string &name) {
  auto it = scopes.find(name);
  if (it != scopes.end()) {
    return it->second;
  }

  Logger::warn("Accessed to an undefined scope `" + name + "`");
  return nullptr;
}
