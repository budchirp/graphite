#include "codegen_llvm/expression/binary.hpp"

#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/options.hpp"
#include "codegen_llvm/utils.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *BinaryExpressionCodegen::codegen() const {
  auto left_value =
      LLVMCodegen::codegen(context, expression->left,
                           expression->op.type == TOKEN_ASSIGN
                               ? make_shared<CodegenOptions>(false, true)
                               : nullptr);
  if (!left_value) {
    Logger::error(
        "Failed to generate low level code for left hand side expression",
        LogTypes::Error::INTERNAL, expression->left->get_position());
    return nullptr;
  }

  if (expression->op.type == TOKEN_AS) return left_value;

  auto right_value = LLVMCodegen::codegen(context, expression->right);
  if (!right_value) {
    Logger::error(
        "Failed to generate low level code for right hand side expression",
        LogTypes::Error::INTERNAL, expression->right->get_position());
    return nullptr;
  }

  auto type = expression->right->get_type();
  auto llvm_type = LLVMCodegenUtils::type_to_llvm_type(context, type);
  if (expression->op.type != TOKEN_ASSIGN)
    if (auto casted = LLVMCodegenUtils::cast_type(
            context, left_value, llvm_type,
            !(expression->op.type == TOKEN_EQUAL ||
              expression->op.type == TOKEN_NOT_EQUAL))) {
      left_value = casted;
    }

  switch (expression->op.type) {
    case TOKEN_ASSIGN: {
      context->builder->CreateStore(right_value, left_value);
      return right_value;
    }

    default: {
      if (TypeHelper::is_boolean(type)) {
        switch (expression->op.type) {
          case TOKEN_AND:
            return context->builder->CreateAnd(left_value, right_value, "and");
          case TOKEN_OR:
            return context->builder->CreateOr(left_value, right_value, "or");
          case TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          default:
            return nullptr;
        }
      }

      if (TypeHelper::is_int(type)) {
        switch (expression->op.type) {
          case TOKEN_PLUS:
            return context->builder->CreateAdd(left_value, right_value, "add");
          case TOKEN_MINUS:
            return context->builder->CreateSub(left_value, right_value, "sub");
          case TOKEN_ASTERISK:
            return context->builder->CreateMul(left_value, right_value, "mul");
          case TOKEN_SLASH:
            return context->builder->CreateSDiv(left_value, right_value, "div");
          case TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          case TOKEN_LESS_THAN:
            return context->builder->CreateICmpSLT(left_value, right_value,
                                                   "lt");
          case TOKEN_GREATER_THAN:
            return context->builder->CreateICmpSGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      }

      if (TypeHelper::is_float(type)) {
        switch (expression->op.type) {
          case TOKEN_PLUS:
            return context->builder->CreateFAdd(left_value, right_value,
                                                "fadd");
          case TOKEN_MINUS:
            return context->builder->CreateFSub(left_value, right_value,
                                                "fsub");
          case TOKEN_ASTERISK:
            return context->builder->CreateFMul(left_value, right_value,
                                                "fmul");
          case TOKEN_SLASH:
            return context->builder->CreateFDiv(left_value, right_value,
                                                "fdiv");
          case TOKEN_EQUAL:
            return context->builder->CreateFCmpUEQ(left_value, right_value,
                                                   "eq");
          case TOKEN_NOT_EQUAL:
            return context->builder->CreateFCmpUNE(left_value, right_value,
                                                   "ne");
          case TOKEN_LESS_THAN:
            return context->builder->CreateFCmpULT(left_value, right_value,
                                                   "lt");
          case TOKEN_GREATER_THAN:
            return context->builder->CreateFCmpUGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      }

      if (TypeHelper::is_string(type)) {
        switch (expression->op.type) {
          case TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "streq");
          case TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "strne");
          default:
            return nullptr;
        }
      }

      if (TypeHelper::is_null(type) || TypeHelper::is_pointer(type)) {
        switch (expression->op.type) {
          case TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TOKEN_NOT_EQUAL:
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
