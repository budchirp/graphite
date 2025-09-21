#pragma once

#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Target/TargetMachine.h>

#include <memory>

#include "program/context.hpp"

using namespace std;

class LLVMCodegenContext {
 private:
  shared_ptr<ProgramContext> program_context;

  unordered_map<string, llvm::Value *> variables;

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

  explicit LLVMCodegenContext(
      const shared_ptr<ProgramContext> &program_context);
  ~LLVMCodegenContext() { delete target_machine; }

  shared_ptr<ProgramContext> get_program_context() const {
    return program_context;
  }

  shared_ptr<Env> get_env() const { return program_context->get_env(); }


  void add_variable(const string &name, llvm::Value *variable);

  llvm::Value *get_variable(const string &name) const;
  llvm::Value *get_variable(const shared_ptr<VariableSymbol> &variable) const;
  llvm::Value* get_variable_value(llvm::Value* value, const shared_ptr<Type> &type) const;
  llvm::Value *get_variable_value(
      const shared_ptr<VariableSymbol> &variable) const;
};
