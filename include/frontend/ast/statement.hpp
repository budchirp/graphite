#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/lexer/position.hpp"

using namespace std;

class Statement {
public:
  virtual ~Statement() = default;

  virtual llvm::Value *codegen() = 0;

  virtual Position *get_position() = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
