#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>

#include <memory>
#include <vector>

#include "types/type.hpp"

using namespace std;

class FunctionType : public Type {
 public:
  vector<shared_ptr<Type>> parameters;
  shared_ptr<Type> return_type;

  explicit FunctionType(vector<shared_ptr<Type>> parameters,
                        shared_ptr<Type> return_type)
      : parameters(parameters), return_type(return_type) {};

  llvm::FunctionType* to_llvm(
      shared_ptr<llvm::LLVMContext> context) const override {
    vector<llvm::Type*> llvm_parameters;
    for (const auto& parameter : parameters) {
      llvm_parameters.push_back(parameter->to_llvm(context));
    }

    llvm::Type* llvm_return_type = return_type->to_llvm(context);

    return llvm::FunctionType::get(llvm_return_type, llvm_parameters, false);
  }

  string to_string() const override {
    return "fn " + return_type->to_string();
  };
  string to_string_tree() const override {
    return "FunctionType(parameters: , return_type: " +
           return_type->to_string_tree() + ")";
  };
};
