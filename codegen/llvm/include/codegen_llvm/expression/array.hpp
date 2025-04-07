#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/array.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class ArrayExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<ArrayExpression> expression;

 public:
  explicit ArrayExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<ArrayExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
