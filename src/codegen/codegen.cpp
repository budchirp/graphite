#include "codegen/codegen.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

#include <cstdio>
#include <memory>

#include "logger/logger.hpp"

using namespace std;
using namespace llvm;

shared_ptr<llvm::LLVMContext> context;
shared_ptr<llvm::Module> module;
shared_ptr<llvm::IRBuilder<>> builder;
unordered_map<string, Value *> named_values;

Codegen::Codegen(const shared_ptr<Program> &program) {
  this->program = program;

  context = make_shared<LLVMContext>();
  module = make_shared<Module>("graphite", *context);
  builder = make_shared<IRBuilder<>>(*context);
}

string Codegen::generate_ir() const {
  program->codegen();

  string ir_string;
  raw_string_ostream ir_stream(ir_string);
  module->print(ir_stream, nullptr);
  module->print(errs(), nullptr);

  return ir_stream.str();
}

Value *Codegen::cast_type(Value *value, llvm::Type *expectedType) {
  if (value->getType()->isPointerTy() || expectedType->isPointerTy())
    return value;

  if (value->getType() == expectedType) {
    return value;
  }

  if (value->getType()->isIntegerTy() && expectedType->isIntegerTy()) {
    if (value->getType()->getIntegerBitWidth() >
        expectedType->getIntegerBitWidth()) {
      return builder->CreateTrunc(value, expectedType);
    } else {
      return builder->CreateSExt(value, expectedType);
    }
  }

  if (value->getType()->isFloatingPointTy() &&
      expectedType->isFloatingPointTy()) {
    if (value->getType()->getPrimitiveSizeInBits() >
        expectedType->getPrimitiveSizeInBits()) {
      return builder->CreateFPTrunc(value, expectedType);
    } else {
      return builder->CreateFPExt(value, expectedType);
    }
  }

  if (value->getType()->isIntegerTy() && expectedType->isFloatingPointTy()) {
    return builder->CreateSIToFP(value, expectedType);
  }

  if (value->getType()->isFloatingPointTy() && expectedType->isIntegerTy()) {
    return builder->CreateFPToSI(value, expectedType);
  }

  return nullptr;
}
