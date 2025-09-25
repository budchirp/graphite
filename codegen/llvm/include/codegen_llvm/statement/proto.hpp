#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/proto.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class FunctionProtoStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<FunctionProtoStatement> statement;

 public:
  explicit FunctionProtoStatementCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<FunctionProtoStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
  llvm::Function* codegen_proto() const;
};

class VarProtoStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<VarProtoStatement> statement;

 public:
  explicit VarProtoStatementCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<VarProtoStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
};