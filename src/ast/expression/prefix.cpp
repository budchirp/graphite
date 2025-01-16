#include "ast/expression/prefix.hpp"

#include <llvm/IR/Value.h>

#include <string>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *PrefixExpression::codegen() const {
  auto value = right->codegen();
  if (!value) {
    Logger::error("Failed to generate low level code for expression",
                  LogTypes::Error::INTERNAL, right->get_position());
    return nullptr;
  }

  switch (prefix.type) {
    case TokenType::TOKEN_ASTERISK: {
      if (!value->getType()->isPointerTy()) {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      }

      return context->builder->CreateLoad(value->getType(), value, "deref");
    }

    case TokenType::TOKEN_AMPERSAND: {
      AllocaInst *alloca = context->builder->CreateAlloca(
          right->get_type()->to_llvm(context->llvm_context), nullptr, "addr");
      context->builder->CreateStore(value, alloca);

      return alloca;
    }

    default:
      Logger::error("Unsupported operator type in prefix expression",
                    LogTypes::Error::SYNTAX, &position);
      return nullptr;
  }
}

string PrefixExpression::to_string() const {
  return prefix.to_string() + right->to_string();
}

string PrefixExpression::to_string_tree() const {
  return "PrefixExpression(op: '" + prefix.to_string_tree() +
         "', right: " + right->to_string_tree() + ")";
}
