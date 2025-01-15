#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/expression.hpp"

using namespace llvm;

Value *ExpressionStatement::codegen() { return expression->codegen(); }
