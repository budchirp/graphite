#pragma once

#include <llvm/IR/Value.h>
#include <string>

#include "frontend/ast/node.hpp"

using namespace std;

class Statement : public Node {
public:
  virtual ~Statement() = default;

  virtual llvm::Value *codegen() = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
