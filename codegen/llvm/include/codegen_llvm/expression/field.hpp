#pragma once

#include <llvm/IR/Value.h>

#include <memory>

#include "ast/expression/field.hpp"
#include "codegen_llvm/context.hpp"
#include "codegen_llvm/expression/codegen.hpp"

using namespace std;

class FieldExpressionCodegen : public ExpressionCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<FieldExpression> expression;

 public:
  explicit FieldExpressionCodegen(const shared_ptr<LLVMCodegenContext>& context,
                                  const shared_ptr<FieldExpression>& expression)
      : context(context), expression(expression) {};

  llvm::Value* codegen() const override;
};
