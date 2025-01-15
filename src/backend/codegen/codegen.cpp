#include <cstdio>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

#include "backend/codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace std;
using namespace llvm;

unique_ptr<LLVMContext> context;
unique_ptr<Module> module;
unique_ptr<IRBuilder<>> builder;
unordered_map<string, Value *> named_values;

Codegen::Codegen(const shared_ptr<Program> &program) {
  this->program = program;

  context = make_unique<LLVMContext>();
  module = make_unique<Module>("graphite", *context);
  builder = make_unique<IRBuilder<>>(*context);
}

string Codegen::generate_ir() const {
  program->codegen();

  string ir_string;
  raw_string_ostream ir_stream(ir_string);
  module->print(ir_stream, nullptr);
  module->print(errs(), nullptr);

  return ir_stream.str();
}

Type *Codegen::resolve_type_identifier(const string &type_name) {
  if (type_name[0] == '*') {
    return Codegen::resolve_type_identifier(type_name.substr(1));
  }

  if (type_name == "i8")
    return Type::getInt8Ty(*context);
  if (type_name == "i16")
    return Type::getInt16Ty(*context);
  if (type_name == "i32")
    return Type::getInt32Ty(*context);
  if (type_name == "i64")
    return Type::getInt64Ty(*context);
  if (type_name == "u8")
  return Type::getInt8Ty(*context);
  if (type_name == "u16")
    return Type::getInt16Ty(*context);
  if (type_name == "u32")
    return Type::getInt32Ty(*context);
  if (type_name == "u64")
    return Type::getInt64Ty(*context);
  if (type_name == "f16")
    return Type::getHalfTy(*context);
  if (type_name == "f32")
    return Type::getFloatTy(*context);
  if (type_name == "f80")
    return Type::getX86_FP80Ty(*context);
  if (type_name == "void")
    return Type::getVoidTy(*context);
  if (type_name == "boolean")
    return Type::getInt1Ty(*context);
  if (type_name == "string")
    return PointerType::get(Type::getInt8Ty(*context), 0);

  Logger::error("Unknown type: " + type_name);
  return nullptr;
}

Value *Codegen::convert_type(Value *value, Type *expectedType) {
  if (value->getType()->isPointerTy() || expectedType->isPointerTy()) return value;

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
