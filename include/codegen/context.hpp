#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <memory>

#include "ast/program.hpp"

using namespace std;

class CodegenContext {
 public:
  shared_ptr<llvm::LLVMContext> llvm_context;
  shared_ptr<llvm::Module> module;
  shared_ptr<llvm::IRBuilder<>> builder;

  shared_ptr<Program> program;

  unordered_map<string, llvm::Value *> named_values;

  explicit CodegenContext(const shared_ptr<Program> &program);
};
