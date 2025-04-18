#include "codegen_llvm/statement/function.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/statement/block.hpp"
#include "codegen_llvm/statement/proto.hpp"
#include "logger/logger.hpp"

llvm::Value *FunctionStatementCodegen::codegen() const {
  return codegen_function();
}
llvm::Function *FunctionStatementCodegen::codegen_function() const {
  auto name = statement->proto->name->value;
  auto function = context->get_env()->get_function(name);

  auto llvm_function = context->module->getFunction(name);
  if (llvm_function) {
    Logger::warn("Function `" + name + "` exists", LogTypes::Warn::SUGGESTION,
                 statement->proto->name->get_position());
  } else {
    llvm_function =
        ProtoStatementCodegen(context, statement->proto).codegen_proto();
    if (!llvm_function) {
      Logger::error(
          "Failed to generate low level code for function `" + name + "`",
          LogTypes::Error::INTERNAL, statement->proto->get_position());
      return nullptr;
    }
  }

  context->get_env()->set_current_scope(statement->scope->get_name());

  for (auto &argument : llvm_function->args()) {
    context->add_variable(argument.getName().str(), &argument);
  }

  auto body_block = BlockStatementCodegen(context, statement->body)
                        .codegen_block(llvm_function, "entry");
  if (!body_block) {
    Logger::error("Failed to generate low level code for function body",
                  LogTypes::Error::INTERNAL, statement->body->get_position());
    llvm_function->eraseFromParent();
    return nullptr;
  }

  if (!body_block->getTerminator()) {
    if (llvm_function->getReturnType()->isVoidTy()) {
      context->builder->CreateRetVoid();
    } else {
      Logger::error("Non-void function missing return statement",
                    LogTypes::Error::UNKNOWN,
                    statement->proto->return_type->get_position());
      llvm_function->eraseFromParent();
      return nullptr;
    }
  }

  context->get_env()->set_current_scope(
      statement->scope->get_parent()->get_name());

  return llvm_function;
}
