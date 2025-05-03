#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/assembly.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class AssemblyExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<AssemblyExpression> expression;

 public:
  explicit AssemblyExpressionCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<AssemblyExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
