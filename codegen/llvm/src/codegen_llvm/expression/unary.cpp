#include "codegen_llvm/expression/unary.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *UnaryExpressionCodegen::codegen() const {
  auto type = expression->get_type();
  auto llvm_type = LLVMCodegenUtils::type_to_llvm_type(context, type);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(expression->identifier->value);

  auto variable_value = context->get_variable_value(variable);
  auto variable_ptr = context->get_variable_ptr(variable);

  switch (expression->op.type) {
    case TOKEN_ASTERISK: {
      return context->builder->CreateLoad(llvm_type, variable_value, "load");
    }

    case TOKEN_AMPERSAND: {
      llvm::AllocaInst *alloca =
          context->builder->CreateAlloca(llvm_type, nullptr, "addr");
      context->builder->CreateStore(variable_value, alloca);

      return alloca;
    }

    case TOKEN_MINUS: {
      if (TypeHelper::is_int(type)) {
        return context->builder->CreateNeg(variable_value, "neg");
      }

      if (TypeHelper::is_float(type)) {
        return context->builder->CreateFNeg(variable_value, "neg");
      }

      return nullptr;
    }

    case TOKEN_PLUS: {
      return variable_value;
    }

    case TOKEN_BANG: {
      return context->builder->CreateNot(variable_value, "not");
    }

    case TOKEN_MINUSMINUS: {
      llvm::Value *value;
      if (TypeHelper::is_int(type)) {
        value = context->builder->CreateSub(
            variable_value, llvm::ConstantInt::get(llvm_type, 1), "dec");
      } else {
        value = context->builder->CreateFSub(
            variable_value, llvm::ConstantFP::get(llvm_type, 1.0), "dec");
      }

      context->builder->CreateStore(value, variable_ptr);
      return variable_value;
    }

    case TOKEN_PLUSPLUS: {
      llvm::Value *value;
      if (TypeHelper::is_int(type)) {
        value = context->builder->CreateAdd(
            variable_value, llvm::ConstantInt::get(llvm_type, 1), "inc");
      } else {
        value = context->builder->CreateFAdd(
            variable_value, llvm::ConstantFP::get(llvm_type, 1.0), "inc");
      }

      context->builder->CreateStore(value, variable_ptr);
      return variable_value;
    }

    default:
      return nullptr;
  }
}
