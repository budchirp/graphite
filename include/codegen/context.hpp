#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <memory>

#include "ast/program_context.hpp"

using namespace std;

class CodegenContext {
 private:
  shared_ptr<ProgramContext> program_context;

 public:
  shared_ptr<llvm::LLVMContext> llvm_context;
  shared_ptr<llvm::Module> module;
  shared_ptr<llvm::IRBuilder<>> builder;

  unordered_map<string, llvm::Value *> value_map;

  explicit CodegenContext(const shared_ptr<ProgramContext> &program_context);
  ~CodegenContext() { value_map.clear(); }

  shared_ptr<ProgramContext> get_program_context() const {
    return program_context;
  }
};
