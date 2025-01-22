#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include <memory>
#include <unordered_map>

#include "types/function.hpp"
#include "types/type.hpp"

using namespace std;

struct EnvVariable {
  shared_ptr<Type> type;
  llvm::Value *value;
  bool is_mutable;

  explicit EnvVariable(const shared_ptr<Type> &type, bool is_mutable)
      : type(type), value(nullptr), is_mutable(is_mutable) {}

  void add_llvm_value(llvm::Value *value) { this->value = value; }
};

struct EnvFunction {
  shared_ptr<FunctionType> type;
  llvm::Function *function;

  explicit EnvFunction(const shared_ptr<FunctionType> &type)
      : type(type), function(nullptr) {}

  void add_llvm_value(llvm::Function *function) { this->function = function; }
};

class Env {
 private:
  shared_ptr<Env> parent;

  unordered_map<string, shared_ptr<Type>> type_names;
  unordered_map<string, shared_ptr<EnvVariable>> globals;
  unordered_map<string, shared_ptr<EnvVariable>> variables;
  unordered_map<string, shared_ptr<EnvFunction>> functions;

 public:
  explicit Env(const shared_ptr<Env> &parent) : parent(parent) {};

  void init();

  void add_type(const string &name, const shared_ptr<Type> &type);
  shared_ptr<Type> get_type(const string &name) const;

  void add_variable(const string &name,
                    const shared_ptr<EnvVariable> &variable);
  shared_ptr<EnvVariable> get_variable(const string &name) const;

  void add_function(const string &name,
                    const shared_ptr<EnvFunction> &function);
  shared_ptr<EnvFunction> get_function(const string &name) const;

  shared_ptr<Env> get_parent() const { return parent; };
};
