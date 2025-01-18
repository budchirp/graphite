#include "ast/expression/prefix.hpp"

#include <llvm/IR/Value.h>

#include <string>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "token/token_type.hpp"

using namespace llvm;

Value *PrefixExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto value = right->codegen(context);
  if (!value) {
    Logger::error("Failed to generate low level code for expression",
                  LogTypes::Error::INTERNAL, right->get_position());
    return nullptr;
  }

  switch (prefix.type) {
    case TOKEN_ASTERISK: {
      if (!value->getType()->isPointerTy()) {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, right->get_position());
        return nullptr;
      }

      return context->builder->CreateLoad(value->getType(), value, "deref");
    }

    case TOKEN_AMPERSAND: {
      AllocaInst *alloca = context->builder->CreateAlloca(
          right->get_type()->to_llvm(context->llvm_context), nullptr, "addr");
      context->builder->CreateStore(value, alloca);

      return alloca;
    }

    default:
      Logger::error("Unsupported operator in prefix expression",
                    LogTypes::Error::SYNTAX, &position);
      return nullptr;
  }
}

void PrefixExpression::analyze(const shared_ptr<ProgramContext> &context) {
  right->analyze(context);

  switch (prefix.type) {
    case TOKEN_ASTERISK: {
      if (!Analyzer::is_pointer(right->get_type()).first) {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, right->get_position());
        return;
      }
    }

    case TOKEN_AMPERSAND: {
      break;
    }

    default: {
      Logger::error("Unsupported operator in prefix expression",
                    LogTypes::Error::SYNTAX, &position);
      return;
    }
  }
}

string PrefixExpression::to_string() const {
  return prefix.to_string() + right->to_string();
}

string PrefixExpression::to_string_tree() const {
  return "PrefixExpression(type: " + type->to_string_tree() + ", op: '" +
         prefix.to_string_tree() + "', right: " + right->to_string_tree() + ")";
}
