#include "codegen/context.hpp"

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

#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "logger/logger.hpp"

static llvm::ExitOnError ExitOnErr;

CodegenContext::CodegenContext(
    const shared_ptr<ProgramContext>& program_context)
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
  module =
      make_shared<llvm::Module>(program_context->get_name(), *llvm_context);
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
