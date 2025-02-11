#pragma once

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "types/type.hpp"

using namespace std;

class UnknownParserType : public Type {
 public:
  string value;

  explicit UnknownParserType(const string &value) : value(value) {};

  llvm::Type *to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    return nullptr;
  };

  string get_name() const override { return "unknown"; }
  const type_info &get_type_info() const override {
    return typeid(UnknownParserType);
  }

  string to_string() const override { return "unknown " + value; };
  string to_string_tree() const override {
    return "UnknownParserType(value: " + value + ")";
  };
};
