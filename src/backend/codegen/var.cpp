#include <iostream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/var.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *VarStatement::codegen() {
  bool is_pointer = type->get_value()[0] == '*';

  Type *llvm_type = Codegen::resolve_type_identifier(type->get_value());
  if (!llvm_type) {
    Logger::error("Failed to resolve type for variable " + name->get_value(), LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  Value *value = expression->codegen();
  if (!value) {
    Logger::error("Failed to generate initializer for variable " +
                  name->get_value(), LogTypes::Error::UNKNOWN, &position);
    return nullptr;
  }

  if (isa<PHINode>(value)) {
    auto phi = cast<PHINode>(value);
    for (auto i = 0; i < phi->getNumIncomingValues(); ++i) {
      auto incoming_value =
          Codegen::convert_type(phi->getIncomingValue(i), llvm_type);
      if (!incoming_value) {
        Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      } else {
        if (is_pointer && !incoming_value->getType()->isPointerTy()) {
          Logger::error("Expected pointer but received value", LogTypes::Error::TYPE_MISMATCH, &position);
          return nullptr;
        } else if (!is_pointer && incoming_value->getType()->isPointerTy()) {
          Logger::error("Expected value but received pointer", LogTypes::Error::TYPE_MISMATCH, &position);
          return nullptr;
        }
      }
    }
  }

  value = Codegen::convert_type(value, llvm_type);
  if (!value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  } else {
    if (is_pointer && !value->getType()->isPointerTy()) {
      Logger::error("Expected pointer but received value", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    } else if (!is_pointer && value->getType()->isPointerTy()) {
      Logger::error("Expected value but received pointer", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    }
  }

  named_values[name->get_value()] = value;

  return value;
}
