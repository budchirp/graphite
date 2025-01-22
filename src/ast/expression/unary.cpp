#include "ast/expression/unary.hpp"

#include <llvm/IR/Value.h>

#include <cmath>
#include <memory>
#include <string>

#include "analyzer/analyzer.hpp"
#include "ast/expression/var_ref.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "token/token_type.hpp"
#include "types/boolean.hpp"
#include "types/int.hpp"

using namespace llvm;

Value *UnaryExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto value = expression->codegen(context);
  if (!value) {
    Logger::error("Failed to generate low level code for expression",
                  LogTypes::Error::INTERNAL, expression->get_position());
    return nullptr;
  }

  auto type = expression->get_type();
  auto llvm_type = type->to_llvm(context->llvm_context);

  switch (op.type) {
    case TOKEN_ASTERISK: {
      return context->builder->CreateLoad(llvm_type, value, "load");
    }

    case TOKEN_AMPERSAND: {
      AllocaInst *alloca =
          context->builder->CreateAlloca(llvm_type, nullptr, "addr");
      context->builder->CreateStore(value, alloca);

      return alloca;
    }

    case TOKEN_MINUS: {
      if (Analyzer::is_int(type).first) {
        return context->builder->CreateNeg(value, "neg");
      } else if (Analyzer::is_float(type).first) {
        return context->builder->CreateFNeg(value, "neg");
      }

      return nullptr;
    }

    case TOKEN_PLUS: {
      return value;
    }

    case TOKEN_BANG: {
      return context->builder->CreateNot(value, "not");
    }

    case TOKEN_MINUSMINUS: {
      Value *new_value;
      if (Analyzer::is_int(type).first) {
        new_value = context->builder->CreateSub(
            value, ConstantInt::get(llvm_type, 1), "dec");
      } else {
        new_value = context->builder->CreateFSub(
            value, ConstantFP::get(llvm_type, 1.0), "dec");
      }

      auto ptr =
          context->get_env()->get_variable(expression->get_name())->value;
      context->builder->CreateStore(new_value, ptr);

      return ptr;
      break;
    }

    case TOKEN_PLUSPLUS: {
      Value *new_value;
      if (Analyzer::is_int(type).first) {
        new_value = context->builder->CreateAdd(
            value, ConstantInt::get(llvm_type, 1), "inc");
      } else {
        new_value = context->builder->CreateFAdd(
            value, ConstantFP::get(llvm_type, 1.0), "inc");
      }

      auto ptr =
          context->get_env()->get_variable(expression->get_name())->value;
      context->builder->CreateStore(new_value, ptr);

      return ptr;
    }

    default:
      return nullptr;
  }
}

void UnaryExpression::analyze(const shared_ptr<ProgramContext> &context) {
  expression->analyze(context);

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      if (!Analyzer::compare(expression->get_type(),
                             make_shared<BooleanType>())) {
        Logger::error("Bang operator only supported with booleans",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return;
      }

      break;
    }

    case TOKEN_MINUSMINUS:
    case TOKEN_PLUSPLUS:
    case TOKEN_MINUS:
    case TOKEN_PLUS: {
      if (!context->get_env()
               ->get_variable(expression->get_name())
               ->is_mutable) {
        Logger::error("Cannot mutate an immutable variable",
                      LogTypes::Error::SYNTAX, &position);
        return;
      }

      if (!Analyzer::compare(expression->get_type(),
                             make_shared<IntType>(32, false))) {
        Logger::error(
            op.literal + " operator only supported with integer or floats",
            LogTypes::Error::TYPE_MISMATCH, &position);
        return;
      }

      break;
    }

    default: {
      Logger::error("Unsupported operator in unary expression",
                    LogTypes::Error::SYNTAX, &position);
      return;
    }
  }
}

string UnaryExpression::to_string() const {
  return op.to_string() + expression->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "PrefixExpression(type: " + type->to_string_tree() + ", op: '" +
         op.to_string_tree() + "', right: " + expression->to_string_tree() +
         ")";
}
