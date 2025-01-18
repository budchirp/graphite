#pragma once

#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

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

  shared_ptr<llvm::FunctionPassManager> fpm;
  shared_ptr<llvm::LoopAnalysisManager> lam;
  shared_ptr<llvm::FunctionAnalysisManager> fam;
  shared_ptr<llvm::CGSCCAnalysisManager> cgam;
  shared_ptr<llvm::ModuleAnalysisManager> mam;
  shared_ptr<llvm::PassInstrumentationCallbacks> pic;
  shared_ptr<llvm::StandardInstrumentations> si;

  llvm::TargetMachine *target_machine;

  unordered_map<string, llvm::Value *> value_map;

  explicit CodegenContext(const shared_ptr<ProgramContext> &program_context);
  ~CodegenContext() {
    value_map.clear();

    delete target_machine;
  }

  shared_ptr<ProgramContext> get_program_context() const {
    return program_context;
  }
};
