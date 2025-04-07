#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "position.hpp"
#include "program/context.hpp"
#include "types/type.hpp"

using namespace std;

class Statement {
 public:
  virtual ~Statement() = default;

  virtual void validate(const shared_ptr<ProgramContext> &context) = 0;
  virtual void resolve_types(const shared_ptr<ProgramContext> &context) = 0;

  virtual Position *get_position() = 0;
  virtual shared_ptr<Type> get_type() const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
