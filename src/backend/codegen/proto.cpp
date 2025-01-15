#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/proto.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *ProtoStatement::codegen() { return codegen_function(); }
Function *ProtoStatement::codegen_function() {
  vector<Type *> parameter_types;
  for (const auto &parameter : parameters) {
    Type *resolved_type = Codegen::resolve_type_identifier(parameter.second->get_value());
    if (!resolved_type) {
      Logger::error("Failed to resolve type for argument " +
                    parameter.first->get_value(), LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    }

    if (parameter.second->get_value()[0] == '*') {
      resolved_type = PointerType::get(resolved_type, 0);
    }

    parameter_types.push_back(resolved_type);
  }

  bool is_pointer = return_type->get_value()[0] == '*';
  Type *return_type = Codegen::resolve_type_identifier(this->return_type->get_value());
  if (!return_type) {
    Logger::error("Failed to resolve return type for function " +
                  name->get_value(), LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  auto function_type = FunctionType::get(
      is_pointer ? PointerType::get(return_type, 0) : return_type,
      parameter_types, false);
  auto function = Function::Create(function_type, Function::ExternalLinkage,
                                   name->get_value(), module.get());

  int idx = 0;
  for (auto &argument : function->args())
    argument.setName(parameters[idx++].first->get_value());

  return function;
}
