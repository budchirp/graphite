#include "codegen_llvm/statement/extern.hpp"

#include "codegen_llvm/statement/proto.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

llvm::Value *ExternStatementCodegen::codegen() const {
  if (auto function_proto_statement =
          dynamic_pointer_cast<FunctionProtoStatement>(statement->proto)) {
    return FunctionProtoStatementCodegen(context, function_proto_statement)
        .codegen_proto();
  } else if (auto var_proto_statement =
                 dynamic_pointer_cast<VarProtoStatement>(statement->proto)) {
    return VarProtoStatementCodegen(context, var_proto_statement).codegen();
  }

  Logger::error("Unknown proto statement", LogTypes::Error::INTERNAL,
                statement->proto->get_position());
  return nullptr;
}
