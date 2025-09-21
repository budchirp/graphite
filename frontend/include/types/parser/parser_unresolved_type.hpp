#pragma once

#include "types/type.hpp"

using namespace std;

class ParserUnresolvedType : public Type {
 public:
  string value;

  explicit ParserUnresolvedType(const string &value) : value(value) {};

  string get_name() const override { return "unresolved"; }

  string to_string() const override { return "unresolved " + value; };
  string to_string_tree() const override {
    return "ParserUnresolvedType(value: '" + value + "')";
  };
};
