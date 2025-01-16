#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <cstdio>
#include <memory>

#include "codegen/context.hpp"
#include "codegen/codegen.hpp"

using namespace std;
using namespace llvm;

shared_ptr<CodegenContext> context;

Codegen::Codegen(const shared_ptr<CodegenContext> &_context) {
  context = _context;
}

string Codegen::generate_ir() const {
  context->program->codegen();

  string ir_string;
  raw_string_ostream ir_stream(ir_string);

  context->module->print(ir_stream, nullptr);
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

  return nullptr;
}
