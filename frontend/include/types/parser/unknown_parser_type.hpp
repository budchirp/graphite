#pragma once

#include "types/type.hpp"

using namespace std;

class UnknownParserType : public Type {
 public:
  string value;

  explicit UnknownParserType(const string &value) : value(value) {};

  string get_name() const override { return "unknown"; }

  string to_string() const override { return "unknown " + value; };
  string to_string_tree() const override {
    return "UnknownParserType(value: '" + value + "')";
  };
};
