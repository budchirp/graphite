#include "ast/statement/proto.hpp"

#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *ProtoStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return codegen_function(context);
}
Function *ProtoStatement::codegen_function(
    const shared_ptr<CodegenContext> &context) const {
  auto function = Function::Create(
      static_cast<FunctionType *>(context->get_program_context()
                                      ->get_env()
                                      ->get_type(name->get_value())
                                      ->to_llvm(context->llvm_context)),
      Function::ExternalLinkage, name->get_value(), context->module.get());

  int idx = 0;
  for (auto &argument : function->args())
    argument.setName(parameters[idx++].first->get_value());

  return function;
}

void ProtoStatement::analyze(const shared_ptr<ProgramContext> &context) {
  for (const auto &parameter : parameters) {
    parameter.second->analyze(context);
  }

  return_type->analyze(context);
}

string ProtoStatement::to_string() const {
  ostringstream oss;
  oss << name->to_string() << "(";

  for (auto i = 0; i < parameters.size(); ++i) {
    oss << parameters[i].first->to_string() << ": "
        << parameters[i].second->to_string();
    if (i < parameters.size() - 1) {
      oss << ", ";
    }
  }

  oss << ") -> " << return_type->to_string();
  return oss.str();
}

string ProtoStatement::to_string_tree() const {
  ostringstream oss;
  oss << "ProtoStatement(name: " << name->to_string_tree() << ", parameters: [";

  for (auto i = 0; i < parameters.size(); ++i) {
    oss << "(value: " << parameters[i].first->to_string_tree()
        << ", type: " << parameters[i].second->to_string_tree() << ")";
    if (i < parameters.size() - 1) {
      oss << ", ";
    }
  }

  oss << "], return_type: " << return_type->to_string_tree() << ")";
  return oss.str();
}
