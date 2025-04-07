
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/statement/proto.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/statement/codegen.hpp"

using namespace std;

class ProtoStatementCodegen : public StatementCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<ProtoStatement> statement;

 public:
  explicit ProtoStatementCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                 const shared_ptr<ProtoStatement>& statement)
      : context(context), statement(statement) {};

  llvm::Value* codegen() const override;
  llvm::Function* codegen_proto() const;
};
