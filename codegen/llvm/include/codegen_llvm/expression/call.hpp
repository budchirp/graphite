#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/call.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class CallExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<CallExpression> expression;

 public:
  explicit CallExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                 const shared_ptr<CallExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
