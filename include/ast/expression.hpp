#pragma once

#include <llvm/IR/Value.h>

#include <string>

#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class Expression {
 public:
  virtual ~Expression() = default;

  virtual llvm::Value *codegen() const = 0;

  virtual Position *get_position() = 0;
  virtual shared_ptr<Type> get_type() const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
