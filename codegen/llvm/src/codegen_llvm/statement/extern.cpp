#include "codegen_llvm/statement/extern.hpp"

#include "codegen_llvm/statement/proto.hpp"

llvm::Value *ExternStatementCodegen::codegen() const {
  return ProtoStatementCodegen(context, statement->proto).codegen_proto();
}
