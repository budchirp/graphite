
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/if.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class IfExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<IfExpression> expression;

 public:
  explicit IfExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                               const shared_ptr<IfExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
