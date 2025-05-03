#include "codegen_llvm/context.hpp"

#include <llvm/IR/PassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include <memory>
#include <string>

#include "codegen_llvm/utils.hpp"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

LLVMCodegenContext::LLVMCodegenContext(
    const shared_ptr<ProgramContext> &program_context)
    : program_context(program_context) {
  auto triple = llvm::sys::getDefaultTargetTriple();

  string error;
  auto target = llvm::TargetRegistry::lookupTarget(triple, error);
  if (!target) {
    Logger::error("Unsupported OS");
    return;
  }

  llvm::TargetOptions options;

  target_machine = target->createTargetMachine(triple, "generic", "", options,
                                               llvm::Reloc::PIC_);

  llvm_context = make_shared<llvm::LLVMContext>();
  module = make_shared<llvm::Module>(program_context->get_module_name(),
                                     *llvm_context);
  module->setDataLayout(target_machine->createDataLayout());
  module->setTargetTriple(triple);
  builder = make_shared<llvm::IRBuilder<>>(*llvm_context);

  fpm = make_shared<llvm::FunctionPassManager>();
  lam = make_shared<llvm::LoopAnalysisManager>();
  fam = make_shared<llvm::FunctionAnalysisManager>();
  cgam = make_shared<llvm::CGSCCAnalysisManager>();
  mam = make_shared<llvm::ModuleAnalysisManager>();
  pic = make_shared<llvm::PassInstrumentationCallbacks>();
  si = make_shared<llvm::StandardInstrumentations>(*llvm_context, true);

  si->registerCallbacks(*pic, mam.get());
}

void LLVMCodegenContext::add_variable(const string &name,
                                      llvm::Value *variable) {
  variables.insert_or_assign(
      program_context->get_env()->get_current_scope()->get_name() + "_" + name,
      variable);
}

llvm::Value *LLVMCodegenContext::get_variable(const string &name) const {
  auto it = variables.find(
      program_context->get_env()->get_current_scope()->get_name() + "_" + name);
  if (it != variables.end() && it->second) {
    return it->second;
  }

  it = variables.find("global_" + name);
  if (it != variables.end() && it->second) {
    return it->second;
  }

  Logger::error("Accessed to an undefined variable `" + name + "`");
  return nullptr;
}

llvm::Value *LLVMCodegenContext::get_variable_value(
    const shared_ptr<VariableSymbol> &variable, const bool &check) const {
  auto value = get_variable(variable->name);

  if (check) {
    if (!(variable->is_mutable || variable->is_global)) return value;
  }

  if (TypeHelper::is_array(variable->type) && variable->is_mutable) {
    auto index_ptr = builder->CreateGEP(
        LLVMCodegenUtils::type_to_llvm_type(this, variable->type), value,
        {builder->getInt32(0), builder->getInt32(0)}, "array_index");

    return index_ptr;
  }

  return builder->CreateLoad(
      LLVMCodegenUtils::type_to_llvm_type(this, variable->type), value);
}

llvm::Value *LLVMCodegenContext::get_variable_ptr(
    const shared_ptr<VariableSymbol> &variable,
    const bool &load_pointer) const {
  if (load_pointer) {
    if (variable->is_mutable || variable->is_global) {
      return get_variable_value(variable, false);
    } else {
      return get_variable(variable->name);
    }
  }

  if (variable->is_mutable || variable->is_global) {
    return get_variable(variable->name);
  }

  return nullptr;
}
