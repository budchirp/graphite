#pragma once

#include <string>
#include <typeinfo>

using namespace std;

class Type {
 public:
  virtual ~Type() = default;

  virtual string get_name() const = 0;
  virtual const type_info& get_type_info() const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
