#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/var_ref.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"
#include "codegen_llvm/options.hpp"

using namespace std;

class VarRefExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<VarRefExpression> expression;

 public:
  explicit VarRefExpressionCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<VarRefExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
  llvm::Value* codegen(const shared_ptr<CodegenOptions> &options) const;
};
