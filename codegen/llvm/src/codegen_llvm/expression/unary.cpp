#include "codegen_llvm/expression/unary.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/options.hpp"
#include "codegen_llvm/utils.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

llvm::Value* UnaryExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value* UnaryExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions>& options) const {
  auto type = expression->get_type();
  auto llvm_type = LLVMCodegenUtils::type_to_llvm_type(context, type);

  switch (expression->op.type) {
    case TOKEN_ASTERISK: {
      auto ptr = LLVMCodegen::codegen(context, expression->expression);
      if (!ptr) {
        Logger::error("Failed to generate low level code for pointer",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      if (options && !options->load_variable) {
        return ptr;
      } else {
        return context->builder->CreateLoad(llvm_type, ptr);
      }
    }

    case TOKEN_AMPERSAND: {
      return LLVMCodegen::codegen(context, expression->expression,
                                  make_shared<CodegenOptions>(false, true));
    }

    case TOKEN_MINUS: {
      auto value = LLVMCodegen::codegen(context, expression->expression);
      if (!value) {
        Logger::error("Failed to generate low level code for value",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      if (TypeHelper::is_int(type)) {
        return context->builder->CreateNeg(value);
      } else if (TypeHelper::is_float(type)) {
        return context->builder->CreateFNeg(value);
      }
      break;
    }

    case TOKEN_PLUS: {
      return LLVMCodegen::codegen(context, expression->expression);
    }

    case TOKEN_BANG: {
      auto value = LLVMCodegen::codegen(context, expression->expression);
      if (!value) {
        Logger::error("Failed to generate low level code for value",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      return context->builder->CreateNot(value);
    }

    case TOKEN_MINUSMINUS: {
      auto ptr = LLVMCodegen::codegen(context, expression->expression,
                                      make_shared<CodegenOptions>(false, true));
      if (!ptr) {
        Logger::error("Failed to generate low level code for pointer",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      auto current_value = context->builder->CreateLoad(llvm_type, ptr);

      llvm::Value* new_value = nullptr;
      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateSub(
            current_value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        new_value = context->builder->CreateFSub(
            current_value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      if (!new_value) {
        Logger::error("Failed to generate low level code for minus minus",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      context->builder->CreateStore(new_value, ptr);
      return new_value;

      break;
    }

    case TOKEN_PLUSPLUS: {
      auto ptr = LLVMCodegen::codegen(context, expression->expression,
                                      make_shared<CodegenOptions>(false, true));
      if (!ptr) {
        Logger::error("Failed to generate low level code for pointer",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      auto current_value = context->builder->CreateLoad(llvm_type, ptr);

      llvm::Value* new_value = nullptr;
      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateAdd(
            current_value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        new_value = context->builder->CreateFAdd(
            current_value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      if (!new_value) {
        Logger::error("Failed to generate low level code for plus plus",
                      LogTypes::Error::INTERNAL,
                      expression->expression->get_position());
        return nullptr;
      }

      context->builder->CreateStore(new_value, ptr);
      return new_value;

      break;
    }

    default:
      break;
  }

  return nullptr;
}