#include "codegen_llvm/expression/unary.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *UnaryExpressionCodegen::codegen() const {
  auto value = LLVMCodegen::codegen(context, expression->var_ref);
  if (!value) {
    Logger::error("Failed to generate low level code for expression",
                  LogTypes::Error::INTERNAL, expression->get_position());
    return nullptr;
  }

  auto type = expression->get_type();
  auto llvm_type = LLVMCodegenUtils::type_to_llvm_type(context,type);

  auto scope = context->get_env()->get_current_scope();

  switch (expression->op.type) {
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
      }

      if (TypeHelper::is_float(type)) {
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

      auto ptr = context->get_variable(expression->var_ref->name);
      context->builder->CreateStore(new_value, ptr);

      return ptr;
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

      auto ptr = context->get_variable(expression->var_ref->name);
      context->builder->CreateStore(new_value, ptr);

      return ptr;
    }

    default:
      return nullptr;
  }
}
