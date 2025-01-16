#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include <memory>
#include <string>

using namespace std;

class Type {
 public:
  virtual ~Type() = default;

  virtual llvm::Type* to_llvm(shared_ptr<llvm::LLVMContext> context) const = 0;

  virtual string to_string() const = 0;
  virtual string to_string_tree() const = 0;
};
