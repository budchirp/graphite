
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/unary.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class UnaryExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<UnaryExpression> expression;

 public:
  explicit UnaryExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<UnaryExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
