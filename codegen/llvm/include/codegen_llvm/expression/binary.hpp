#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/binary.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class BinaryExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<BinaryExpression> expression;

 public:
  explicit BinaryExpressionCodegen(
      const shared_ptr<LLVMCodegenContext> &context,
      const shared_ptr<BinaryExpression> &expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
