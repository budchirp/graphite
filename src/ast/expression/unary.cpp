#include "ast/expression/unary.hpp"

#include <llvm/IR/Value.h>

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

  switch (op.type) {
    case TOKEN_ASTERISK: {
      return context->builder->CreateLoad(
          expression->get_type()->to_llvm(context->llvm_context), value,
          "load");
    }

    case TOKEN_AMPERSAND: {
      AllocaInst *alloca = context->builder->CreateAlloca(
          expression->get_type()->to_llvm(context->llvm_context), nullptr,
          "addr");
      context->builder->CreateStore(value, alloca);

      return alloca;
    }

    case TOKEN_MINUS: {
      auto type = expression->get_type();
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
      auto type = expression->get_type();
      auto llvm_type = type->to_llvm(context->llvm_context);

      Value *new_value;
      if (Analyzer::is_int(type).first) {
        new_value = context->builder->CreateSub(
            value, ConstantInt::get(llvm_type, 1), "dec");
      } else if (Analyzer::is_float(type).first) {
        new_value = context->builder->CreateFSub(
            value, ConstantFP::get(llvm_type, 1.0), "dec");
      }

      if (auto *var = dynamic_cast<VarRefExpression *>(expression.get())) {
        context->builder->CreateStore(new_value,
                                      context->value_map[var->get_value()]);
        new_value = context->value_map[var->get_value()];
      }

      return new_value;
      break;
    }

    case TOKEN_PLUSPLUS: {
      auto type = expression->get_type();
      auto llvm_type = type->to_llvm(context->llvm_context);

      Value *new_value;
      if (Analyzer::is_int(type).first) {
        new_value = context->builder->CreateAdd(
            value, ConstantInt::get(llvm_type, 1), "inc");
      } else if (Analyzer::is_float(type).first) {
        new_value = context->builder->CreateFAdd(
            value, ConstantFP::get(llvm_type, 1.0), "inc");
      }

      if (auto *var = dynamic_cast<VarRefExpression *>(expression.get())) {
        context->builder->CreateStore(new_value,
                                      context->value_map[var->get_value()]);
        new_value = context->value_map[var->get_value()];
      }

      return new_value;
    }

    default:
      return nullptr;
  }
}

void UnaryExpression::analyze(const shared_ptr<ProgramContext> &context) {
  expression->analyze(context);

  switch (op.type) {
    case TOKEN_ASTERISK: {
      if (!Analyzer::is_pointer(expression->get_type()).first) {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return;
      }

      break;
    }

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
