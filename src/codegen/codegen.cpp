#include "codegen/codegen.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include <climits>
#include <cstdio>
#include <memory>

#include "logger/logger.hpp"

using namespace std;
using namespace llvm;

void Codegen::init() {
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
}

void Codegen::codegen(const shared_ptr<Program> &program) const {
  context->get_env()->get_current_scope()->get_variable("null")->add_llvm_value(
      llvm::ConstantInt::getSigned(
          llvm::Type::getInt32Ty(*context->llvm_context), -1));

  program->codegen(context);

  llvm::verifyModule(*context->module);
}

void Codegen::optimize() {
  llvm::PassBuilder pb;

  pb.registerModuleAnalyses(*context->mam);
  pb.registerFunctionAnalyses(*context->fam);
  pb.registerLoopAnalyses(*context->lam);
  pb.registerCGSCCAnalyses(*context->cgam);

  pb.crossRegisterProxies(*context->lam, *context->fam, *context->cgam,
                          *context->mam);

  llvm::ModulePassManager mpm = pb.buildModuleOptimizationPipeline(
      llvm::OptimizationLevel::O3, ThinOrFullLTOPhase::None);

  Logger::log("Optimizing your garbage");
  mpm.run(*context->module, *context->mam);
}

Value *Codegen::cast_type(const shared_ptr<CodegenContext> &context,
                          Value *value, llvm::Type *expectedType, bool strict) {
  if (value->getType()->isPointerTy() || expectedType->isPointerTy())
    return value;

  if (value->getType() == expectedType) {
    return value;
  }

  if (value->getType()->isIntegerTy() && expectedType->isIntegerTy()) {
    if (value->getType()->getIntegerBitWidth() >
        expectedType->getIntegerBitWidth()) {
      return context->builder->CreateTrunc(value, expectedType);
    } else {
      return context->builder->CreateSExt(value, expectedType);
    }
  }

  if (value->getType()->isFloatingPointTy() &&
      expectedType->isFloatingPointTy()) {
    if (value->getType()->getPrimitiveSizeInBits() >
        expectedType->getPrimitiveSizeInBits()) {
      return context->builder->CreateFPTrunc(value, expectedType);
    } else {
      return context->builder->CreateFPExt(value, expectedType);
    }
  }

  if (value->getType()->isIntegerTy() && expectedType->isFloatingPointTy()) {
    return context->builder->CreateSIToFP(value, expectedType);
  }

  if (value->getType()->isFloatingPointTy() && expectedType->isIntegerTy()) {
    return context->builder->CreateFPToSI(value, expectedType);
  }

  return strict ? nullptr : value;
}
