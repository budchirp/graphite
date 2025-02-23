#include "ast/expression/unary.hpp"

#include <llvm/IR/Value.h>

#include <cmath>
#include <memory>
#include <string>

#include "ast/expression/var_ref.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"
#include "types/int.hpp"

llvm::Value *UnaryExpression::codegen(
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
      llvm::AllocaInst *alloca =
          context->builder->CreateAlloca(llvm_type, nullptr, "addr");
      context->builder->CreateStore(value, alloca);

      return alloca;
    }

    case TOKEN_MINUS: {
      if (TypeHelper::is_int(type)) {
        return context->builder->CreateNeg(value, "neg");
      } else if (TypeHelper::is_float(type)) {
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
      llvm::Value *new_value;
      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateSub(
            value, llvm::ConstantInt::get(llvm_type, 1), "dec");
      } else {
        new_value = context->builder->CreateFSub(
            value, llvm::ConstantFP::get(llvm_type, 1.0), "dec");
      }

      auto ptr = context->get_env()
                     ->get_current_scope()
                     ->get_variable(expression->get_name())
                     ->value;
      context->builder->CreateStore(new_value, ptr);

      return ptr;
      break;
    }

    case TOKEN_PLUSPLUS: {
      llvm::Value *new_value;
      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateAdd(
            value, llvm::ConstantInt::get(llvm_type, 1), "inc");
      } else {
        new_value = context->builder->CreateFAdd(
            value, llvm::ConstantFP::get(llvm_type, 1.0), "inc");
      }

      auto ptr = context->get_env()
                     ->get_current_scope()
                     ->get_variable(expression->get_name())
                     ->value;
      context->builder->CreateStore(new_value, ptr);

      return ptr;
    }

    default:
      return nullptr;
  }
}

void UnaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      if (!TypeHelper::compare(expression->get_type(),
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
      auto variable = context->get_env()->get_current_scope()->get_variable(
          expression->get_name());
      if (!variable->is_mutable) {
        Logger::error("Cannot mutate an immutable variable",
                      LogTypes::Error::SYNTAX, &position);
        return;
      }

      if (!TypeHelper::compare(expression->get_type(),
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

void UnaryExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);

  auto expression_type = expression->get_type();

  shared_ptr<Type> type;
  switch (op.type) {
    case TOKEN_BANG_BANG: {
      if (auto null_type = TypeHelper::is_null(expression_type)) {
        type = null_type->child_type;
      } else {
        Logger::error("Trying to make a non-null type non-null", LogTypes::Error::TYPE_MISMATCH, expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_ASTERISK: {
      if (auto pointer_type = TypeHelper::is_pointer(expression_type)) {
        type = pointer_type->pointee_type;
      } else {
        Logger::error("Cannot dereference non-pointer type", LogTypes::Error::TYPE_MISMATCH, expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_AMPERSAND: {
      type = make_shared<PointerType>(expression_type);
      break;
    }

    default:
      type = expression_type;
      break;
  }

  set_type(type);
}

string UnaryExpression::to_string() const {
  return op.to_string() + expression->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "PrefixExpression(type: " + type->to_string_tree() + ", op: '" +
         op.to_string_tree() + "', right: " + expression->to_string_tree() +
         ")";
}
