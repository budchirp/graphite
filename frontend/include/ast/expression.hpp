#pragma once

#include <memory>
#include <string>

#include "position.hpp"
#include "program/context.hpp"
#include "types/type.hpp"

using namespace std;

class Expression {
 public:
  virtual ~Expression() = default;

  virtual void validate(const shared_ptr<ProgramContext> &context) = 0;
  virtual void resolve_types(const shared_ptr<ProgramContext> &context) = 0;

  virtual Position *get_position() = 0;

  virtual void set_type(const shared_ptr<Type> &type) = 0;
  virtual shared_ptr<Type> get_type() const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
