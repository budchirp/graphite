#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>

#include <memory>
#include <vector>

#include "types/type.hpp"

using namespace std;

class FunctionType : public Type {
 private:
  string name = "function";

 public:
  vector<pair<string, shared_ptr<Type>>> parameters;
  shared_ptr<Type> return_type;

  explicit FunctionType(
      const vector<pair<string, shared_ptr<Type>>>& parameters,
      const shared_ptr<Type>& return_type)
      : parameters(parameters), return_type(return_type) {};

  llvm::FunctionType* to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    vector<llvm::Type*> llvm_parameters;
    llvm_parameters.reserve(parameters.size());
    for (const auto& parameter : parameters) {
      llvm_parameters.push_back(parameter.second->to_llvm(context));
    }

    return llvm::FunctionType::get(return_type->to_llvm(context),
                                   llvm_parameters, false);
  }

  string get_name() const override { return name; }
  const type_info& get_type_info() const override {
    return typeid(FunctionType);
  }

  string to_string() const override {
    return "fn " + return_type->to_string();
  };
  string to_string_tree() const override {
    return "FunctionType(parameters: , return_type: " +
           return_type->to_string_tree() + ")";
  };
};
