#pragma once

#include <memory>
#include <unordered_map>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "types/type.hpp"

using namespace std;

class ParserStructType : public Type {
 public:
  unordered_map<shared_ptr<IdentifierExpression>, shared_ptr<TypeExpression>>
      fields;

  explicit ParserStructType(
      const unordered_map<shared_ptr<IdentifierExpression>,
                          shared_ptr<TypeExpression>>& fields)
      : fields(fields) {};

  string get_name() const override { return "unresolved struct"; }

  string to_string() const override {
    // TODO:
    return "unresolved struct {}";
  };
  string to_string_tree() const override {
    // TODO:
    return "ParserStructType(fields: )";
  };
};
