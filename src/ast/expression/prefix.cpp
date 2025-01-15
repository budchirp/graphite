#include "ast/expression/prefix.hpp"

#include <llvm/IR/Value.h>

#include <string>

#include "cli/commands/compile.hpp"
#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *PrefixExpression::codegen() {
  Value *value = right->codegen();

  switch (prefix.type) {
    case TokenType::TOKEN_ASTERISK: {
      if (!value->getType()->isPointerTy()) {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      }

      return builder->CreateLoad(value->getType(), value, "deref");
    }

    case TokenType::TOKEN_AMPERSAND: {
      AllocaInst *alloca = builder->CreateAlloca(
          right->get_type()->to_llvm(context), nullptr, "addr");
      builder->CreateStore(value, alloca);

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
