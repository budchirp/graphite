#include <memory>
#include <llvm/IR/Value.h>

#include "ast/statement/expression.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *ExpressionStatement::codegen(const shared_ptr<CodegenContext> &context) const { return expression->codegen(context); }

void ExpressionStatement::analyze(
    const shared_ptr<ProgramContext> &context) {
  expression->analyze(context);
}

string ExpressionStatement::to_string() const {
  return expression->to_string();
}

string ExpressionStatement::to_string_tree() const {
  return "ExpressionStatement(expression: " + expression->to_string_tree() +
         ")";
}
