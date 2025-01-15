#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/prefix.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *PrefixExpression::codegen() {
  Value *value = right->codegen();

  switch (prefix.type) {
  case TokenType::TOKEN_ASTERISK: {
    if (!value->getType()->isPointerTy()) {
      Logger::error("Cannot dereference non-pointer type", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    }

    return builder->CreateLoad(value->getType(), value, "deref");
  }

  case TokenType::TOKEN_AMPERSAND: {
    if (value->getType()->isPointerTy()) {
      return value;
    } else {
      AllocaInst *alloca =
          builder->CreateAlloca(Codegen::resolve_type_identifier("i32"), nullptr, "constaddr");
      builder->CreateStore(value, alloca);

      return alloca;
    }
  }

  default:
    Logger::error("Unsupported operator type in prefix expression", LogTypes::Error::SYNTAX, &position);
    return nullptr;
  }
}
