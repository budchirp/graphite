#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/return.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *ReturnStatement::codegen() {
  if (Value *value = expression->codegen()) {
    Function *function = builder->GetInsertBlock()->getParent();
    Type *return_type = function->getReturnType();

    value = Codegen::convert_type(value, return_type);
    if (!value) {
      Logger::error("Return type does not match function's return type", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    } else {
      if (return_type->isPointerTy() && !value->getType()->isPointerTy()) {
        Logger::error("Return expected pointer but received value", LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      } else if (!return_type->isPointerTy() &&
                 value->getType()->isPointerTy()) {
        Logger::error("Return expected value but received pointer", LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      } else if (return_type->isPointerTy() &&
                 value->getType()->isPointerTy()) {
        // find a way to check this
        // if () {
        //   Logger::error("Return type does not match function's return type");
        //   return nullptr;
        // }
      }
    }

    builder->CreateRet(value);

    return value;
  } else {
    Logger::error("Failed to generate return value", LogTypes::Error::UNKNOWN, &position);
    return nullptr;
  }
}
