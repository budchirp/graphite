#include "codegen_llvm/statement/proto.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>

#include <memory>

#include "codegen_llvm/utils.hpp"

llvm::Value *ProtoStatementCodegen::codegen() const { return codegen_proto(); }
llvm::Function *ProtoStatementCodegen::codegen_proto() const {
  auto name = statement->name->value;

  auto function = llvm::Function::Create(
      static_cast<llvm::FunctionType *>(LLVMCodegenUtils::type_to_llvm_type(
          context, context->get_env()->get_function(name)->type)),
      context->get_env()->get_function(name)->visibility ==
              SymbolVisibility::Value::PUBLIC
          ? llvm::Function::ExternalLinkage
          : llvm::Function::InternalLinkage,
      name, context->module.get());

  size_t idx = 0;
  for (auto &argument : function->args())
    argument.setName(statement->parameters[idx++].first->value);

  llvm::verifyFunction(*function);

  return function;
}
