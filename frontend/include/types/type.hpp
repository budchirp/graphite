#pragma once

#include <string>

using namespace std;

class Type {
 public:
  virtual ~Type() = default;

  virtual string get_name() const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
