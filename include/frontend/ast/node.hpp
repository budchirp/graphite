#pragma once

#include <llvm/IR/Value.h>
#include <string>

using namespace std;

class Node {
public:
  virtual ~Node() = default;

  virtual llvm::Value *codegen() = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
