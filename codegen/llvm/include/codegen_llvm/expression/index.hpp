
#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/index.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class IndexExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<IndexExpression> expression;

 public:
  explicit IndexExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<IndexExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
