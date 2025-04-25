#pragma once

#include <memory>
#include <vector>

#include "types/type.hpp"

using namespace std;

class FunctionType : public Type {
 public:
  static string name;

  vector<pair<string, shared_ptr<Type>>> parameters;
  shared_ptr<Type> return_type;

  explicit FunctionType(
      const vector<pair<string, shared_ptr<Type>>>& parameters,
      const shared_ptr<Type>& return_type)
      : parameters(parameters), return_type(return_type) {};

  string get_name() const override { return name; }

  string to_string() const override {
    // TODO:
    return "fn " + return_type->to_string();
  };
  string to_string_tree() const override {
    // TODO:
    return "FunctionType(parameters: , return_type: " +
           return_type->to_string_tree() + ")";
  };
};
