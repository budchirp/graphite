#pragma once

#include <memory>
#include <unordered_map>

#include "types/type.hpp"

using namespace std;

class StructType : public Type {
 public:
  static string name;

  unordered_map<string, shared_ptr<Type>> fields;

  explicit StructType(const unordered_map<string, shared_ptr<Type>>& fields)
      : fields(fields) {};

  string get_name() const override { return name; }

  string to_string() const override {
    // TODO:
    return "struct";
  };
  string to_string_tree() const override {
    // TODO:
    return "StructType(fields: )";
  };
};
