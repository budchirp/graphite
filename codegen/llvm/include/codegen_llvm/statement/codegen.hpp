#pragma once

#include <llvm/IR/Value.h>

using namespace std;

class StatementCodegen {
 public:
  virtual ~StatementCodegen() = default;

  virtual llvm::Value *codegen() const = 0;
};
