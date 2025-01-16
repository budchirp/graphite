#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <iostream>
#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"
#include "ast/statement/proto.hpp"

using namespace llvm;

Value *ProtoStatement::codegen() const { return codegen_function(); }
Function *ProtoStatement::codegen_function() const {
  cout << "env in proto: " << program << endl;

  auto function = Function::Create(
      static_cast<FunctionType *>(
          program->get_env()->get_type(name->get_value())->to_llvm(context)),
      Function::ExternalLinkage, name->get_value(), module.get());

  int idx = 0;
  for (auto &argument : function->args())
    argument.setName(parameters[idx++].first->get_value());

  return function;
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
