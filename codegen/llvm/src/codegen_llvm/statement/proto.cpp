#include "codegen_llvm/statement/proto.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Casting.h>

#include <memory>

#include "codegen_llvm/utils.hpp"

llvm::Value *ProtoStatementCodegen::codegen() const { return codegen_proto(); }
llvm::Function *ProtoStatementCodegen::codegen_proto() const {
  auto function = context->get_env()->get_function(statement->name->value);

  auto linkage = function->visibility == SymbolVisibility::Value::PUBLIC
                     ? llvm::Function::ExternalLinkage
                     : llvm::Function::InternalLinkage;

  auto llvm_function_type = static_cast<llvm::FunctionType *>(
      LLVMCodegenUtils::type_to_llvm_type(context, function->type));

  auto llvm_function = llvm::Function::Create(
      llvm_function_type, linkage, function->name, context->module.get());

  size_t idx = 0;
  for (auto &argument : llvm_function->args())
    argument.setName(statement->parameters[idx++].first->value);

  llvm::verifyFunction(*llvm_function);

  return llvm_function;
}
