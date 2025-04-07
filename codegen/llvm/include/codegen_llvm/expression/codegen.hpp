#pragma once

#include <llvm/IR/Value.h>

using namespace std;

class ExpressionCodegen {
 public:
  virtual ~ExpressionCodegen() = default;

  virtual llvm::Value *codegen() const = 0;
};
