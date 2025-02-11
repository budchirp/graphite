#include "ast/expression/binary.hpp"

#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

#include <ios>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "ast/expression/type.hpp"
#include "ast/expression/var_ref.hpp"
#include "codegen/codegen.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/string.hpp"

llvm::Value *BinaryExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto left_value = left->codegen(context);
  if (!left_value) {
    Logger::error(
        "Failed to generate low level code for left hand side expression",
        LogTypes::Error::INTERNAL, left->get_position());
    return nullptr;
  }

  if (op.type == TOKEN_AS) return left_value;

  auto right_value = right->codegen(context);
  if (!right_value) {
    Logger::error(
        "Failed to generate low level code for right hand side expression",
        LogTypes::Error::INTERNAL, right->get_position());
    return nullptr;
  }

  auto type = left->get_type();
  auto llvm_type = type->to_llvm(context->llvm_context);
  right_value = Codegen::cast_type(
      context, right_value, llvm_type,
      !(op.type == TOKEN_EQUAL || op.type == TOKEN_NOT_EQUAL));
  if (!right_value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                  right->get_position());
    return nullptr;
  }

  switch (op.type) {
    case TOKEN_ASSIGN: {
      if (auto *variable_expression =
              dynamic_cast<VarRefExpression *>(left.get())) {
        auto variable = context->get_env()->get_current_scope()->get_variable(
            variable_expression->get_name());
        context->builder->CreateStore(right_value, variable->value);
        return variable->value;
      }

      return nullptr;
    }

    default: {
      if (TypeHelper::is_boolean(type)) {
        switch (op.type) {
          case TokenType::TOKEN_AND:
            return context->builder->CreateAnd(left_value, right_value, "and");
          case TokenType::TOKEN_OR:
            return context->builder->CreateOr(left_value, right_value, "or");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          default:
            return nullptr;
        }
      } else if (TypeHelper::is_int(type)) {
        switch (op.type) {
          case TokenType::TOKEN_PLUS:
            return context->builder->CreateAdd(left_value, right_value, "add");
          case TokenType::TOKEN_MINUS:
            return context->builder->CreateSub(left_value, right_value, "sub");
          case TokenType::TOKEN_ASTERISK:
            return context->builder->CreateMul(left_value, right_value, "mul");
          case TokenType::TOKEN_SLASH:
            return context->builder->CreateSDiv(left_value, right_value, "div");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          case TokenType::TOKEN_LESS_THAN:
            return context->builder->CreateICmpSLT(left_value, right_value,
                                                   "lt");
          case TokenType::TOKEN_GREATER_THAN:
            return context->builder->CreateICmpSGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      } else if (TypeHelper::is_float(type)) {
        switch (op.type) {
          case TokenType::TOKEN_PLUS:
            return context->builder->CreateFAdd(left_value, right_value,
                                                "fadd");
          case TokenType::TOKEN_MINUS:
            return context->builder->CreateFSub(left_value, right_value,
                                                "fsub");
          case TokenType::TOKEN_ASTERISK:
            return context->builder->CreateFMul(left_value, right_value,
                                                "fmul");
          case TokenType::TOKEN_SLASH:
            return context->builder->CreateFDiv(left_value, right_value,
                                                "fdiv");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateFCmpUEQ(left_value, right_value,
                                                   "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateFCmpUNE(left_value, right_value,
                                                   "ne");
          case TokenType::TOKEN_LESS_THAN:
            return context->builder->CreateFCmpULT(left_value, right_value,
                                                   "lt");
          case TokenType::TOKEN_GREATER_THAN:
            return context->builder->CreateFCmpUGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      } else if (TypeHelper::is_string(type)) {
        switch (op.type) {
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "streq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "strne");
          default:
            return nullptr;
        }
      } else if (TypeHelper::is_null(type) || TypeHelper::is_pointer(type)) {
        switch (op.type) {
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          default:
            return nullptr;
        }
      }
    }
  }

  return nullptr;
}

void BinaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  left->validate(context);
  right->validate(context);

  if (op.type == TOKEN_AS) return;

  auto left_type = left->get_type();
  auto right_type = right->get_type();

  if (!TypeHelper::compare(left_type, right_type,
                           op.type == TOKEN_ASSIGN || op.type == TOKEN_EQUAL ||
                               op.type == TOKEN_NOT_EQUAL)) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  switch (op.type) {
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
    case TOKEN_ASSIGN: {
      return;
    }

    default: {
      static const unordered_map<string, vector<TokenType>> operations = {
          {typeid(IntType).name(),
           {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH,
            TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
          {typeid(FloatType).name(),
           {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH,
            TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
          {typeid(BooleanType).name(), {TOKEN_AND, TOKEN_OR}},
          {typeid(StringType).name(), {}},
          {typeid(PointerType).name(), {}},
          {typeid(NullType).name(), {}}};

      auto types_it = operations.find(left_type->get_type_info().name());
      if (types_it == operations.end()) {
        Logger::error("Unsupported type for binary expression",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return;
      }

      auto type_operations = operations.at(left_type->get_type_info().name());
      auto operations_it =
          find(type_operations.begin(), type_operations.end(), op.type);
      if (operations_it == type_operations.end()) {
        Logger::error(
            "Unsupported operation for type `" + left_type->to_string() + "`",
            LogTypes::Error::SYNTAX, &position);
        return;
      }
    }
  }
}

void BinaryExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  left->resolve_types(context);
  right->resolve_types(context);

  switch (op.type) {
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_THAN:
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
      type = make_shared<BooleanType>();
      break;

    case TOKEN_ASSIGN: {
      if (auto variable_expression =
              dynamic_cast<VarRefExpression *>(left.get())) {
        auto variable = context->get_env()->get_current_scope()->get_variable(
            variable_expression->get_name());
        if (!variable->is_initialized /* && !TypeHelper::is_null(variable->actual_type) */) {
          type = dynamic_pointer_cast<NullType>(variable->type)->child_type;
        } else {
          type = variable->type;
        }

        variable->set_type(type);
      } else {
        Logger::error(
            "Expected variable reference as left hand side expression",
            LogTypes::Error::SYNTAX, left->get_position());
      }

      break;
    }

    case TOKEN_AS: {
      if (auto variable_expression =
              dynamic_cast<VarRefExpression *>(left.get())) {
        auto variable = context->get_env()->get_current_scope()->get_variable(
            variable_expression->get_name());
        if (auto type_expression = dynamic_cast<TypeExpression *>(right.get())) {
          type = type_expression->get_type();
        } else {
          Logger::error("Expected type expression as right hand side expression");
          return;
        }
      } else {
        Logger::error(
            "Expected variable reference as left hand side expression",
            LogTypes::Error::SYNTAX, left->get_position());
        return;
      }

      break;
    }

    default: {
      auto left_type = left->get_type();
      auto right_type = right->get_type();
      if (TypeHelper::is_float(left_type)) {
        type = left_type;
      } else if (TypeHelper::is_float(right_type)) {
        type = right_type;
      } else {
        type = left_type;
      }
    }
  }
}

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(type: " + (type ? type->to_string_tree() : "") +
         ", op: " + op.literal + ", left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
