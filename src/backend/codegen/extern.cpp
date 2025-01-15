#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/extern.hpp"

using namespace llvm;

Value *ExternStatement::codegen() { return proto->codegen_function(); }
