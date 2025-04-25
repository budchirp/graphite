#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/struct.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class StructExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<StructExpression> expression;

 public:
  explicit StructExpressionCodegen(
      const shared_ptr<LLVMCodegenContext>& context,
      const shared_ptr<StructExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
