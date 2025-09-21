#include "codegen_llvm/expression/unary.hpp"

#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>
#include <string>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/options.hpp"
#include "codegen_llvm/utils.hpp"
#include "lexer/token/token_type.hpp"
#include "semantic/type_helper.hpp"

llvm::Value* UnaryExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value* UnaryExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions>& options) const {
  auto type = expression->get_type();
  auto llvm_type = LLVMCodegenUtils::type_to_llvm_type(context, type);

  auto ptr = LLVMCodegen::codegen(context, expression->expression,
                                  options ? options
                                  : (expression->op.type == TOKEN_MINUSMINUS ||
                                     expression->op.type == TOKEN_PLUSPLUS ||
                                     expression->op.type == TOKEN_AMPERSAND)
                                      ? make_shared<CodegenOptions>(false, true)
                                      : nullptr);

  switch (expression->op.type) {
    case TOKEN_ASTERISK: {
      return (options && !options->load_pointer)
                 ? ptr
                 : context->get_variable_value(ptr, type);
    }

    case TOKEN_AMPERSAND: {
      return ptr;
    }

    case TOKEN_MINUS: {
      auto value = context->get_variable_value(ptr, type);
      if (TypeHelper::is_int(type)) {
        return context->builder->CreateNeg(value);
      } else if (TypeHelper::is_float(type)) {
        return context->builder->CreateFNeg(value);
      }
    }

    case TOKEN_PLUS: {
      return context->builder->CreateLoad(llvm_type, ptr);
    }

    case TOKEN_BANG: {
      return context->builder->CreateNot(
          context->builder->CreateLoad(llvm_type, ptr));
    }

    case TOKEN_MINUSMINUS: {
      auto value = context->get_variable_value(ptr, type);
      if (TypeHelper::is_int(type)) {
        value = context->builder->CreateSub(
            value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        value = context->builder->CreateFSub(
            value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      context->builder->CreateStore(value, ptr);
      return value;
    }

    case TOKEN_PLUSPLUS: {
      auto value = context->get_variable_value(ptr, type);
      if (TypeHelper::is_int(type)) {
        value = context->builder->CreateAdd(
            value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        value = context->builder->CreateFAdd(
            value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      context->builder->CreateStore(value, ptr);
      return value;
    }

    default:
      return nullptr;
  }
}
