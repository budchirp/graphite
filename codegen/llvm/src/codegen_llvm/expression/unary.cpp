#include "codegen_llvm/expression/unary.hpp"

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

  switch (expression->op.type) {
    case TOKEN_ASTERISK: {
      // Pointer dereference: *ptr
      // Get the pointer value (not its address)
      auto ptr_value = LLVMCodegen::codegen(context, expression->expression);
      if (!ptr_value) return nullptr;

      // ptr_value now contains the address to dereference
      if (options && !options->load_variable) {
        // Return the address for assignment: *ptr = value
        return ptr_value;
      } else {
        // Return the dereferenced value: x = *ptr
        return context->builder->CreateLoad(llvm_type, ptr_value);
      }
    }

    case TOKEN_AMPERSAND: {
      // Address-of: &var
      // Get the address of the operand
      auto operand_options = make_shared<CodegenOptions>(false, true);
      return LLVMCodegen::codegen(context, expression->expression,
                                  operand_options);
    }

    case TOKEN_MINUS: {
      // Unary minus: -value
      auto value = LLVMCodegen::codegen(context, expression->expression);
      if (!value) return nullptr;

      if (TypeHelper::is_int(type)) {
        return context->builder->CreateNeg(value);
      } else if (TypeHelper::is_float(type)) {
        return context->builder->CreateFNeg(value);
      }
      break;
    }

    case TOKEN_PLUS: {
      // Unary plus: +value (just return the value)
      return LLVMCodegen::codegen(context, expression->expression);
    }

    case TOKEN_BANG: {
      // Logical not: !value
      auto value = LLVMCodegen::codegen(context, expression->expression);
      if (!value) return nullptr;
      return context->builder->CreateNot(value);
    }

    case TOKEN_MINUSMINUS: {
      // Decrement: --var (prefix) or var-- (postfix)
      auto var_options = make_shared<CodegenOptions>(false, true);
      auto var_address =
          LLVMCodegen::codegen(context, expression->expression, var_options);
      if (!var_address) return nullptr;

      auto current_value = context->builder->CreateLoad(llvm_type, var_address);
      llvm::Value* new_value = nullptr;

      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateSub(
            current_value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        new_value = context->builder->CreateFSub(
            current_value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      if (new_value) {
        context->builder->CreateStore(new_value, var_address);
        return new_value;
      }
      break;
    }

    case TOKEN_PLUSPLUS: {
      // Increment: ++var (prefix) or var++ (postfix)
      auto var_options = make_shared<CodegenOptions>(false, true);
      auto var_address =
          LLVMCodegen::codegen(context, expression->expression, var_options);
      if (!var_address) return nullptr;

      auto current_value = context->builder->CreateLoad(llvm_type, var_address);
      llvm::Value* new_value = nullptr;

      if (TypeHelper::is_int(type)) {
        new_value = context->builder->CreateAdd(
            current_value, llvm::ConstantInt::get(llvm_type, 1));
      } else if (TypeHelper::is_float(type)) {
        new_value = context->builder->CreateFAdd(
            current_value, llvm::ConstantFP::get(llvm_type, 1.0));
      }

      if (new_value) {
        context->builder->CreateStore(new_value, var_address);
        return new_value;
      }
      break;
    }

    default:
      break;
  }

  return nullptr;
}