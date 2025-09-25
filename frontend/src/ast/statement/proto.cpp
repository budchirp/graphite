#include "ast/statement/proto.hpp"

#include <cstddef>
#include <memory>
#include <sstream>

void FunctionProtoStatement::validate(
    const shared_ptr<ProgramContext> &context) {
  for (const auto &parameter : parameters) {
    parameter.second->validate(context);
  }

  return_type->validate(context);
}

void FunctionProtoStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  for (const auto &parameter : parameters) {
    parameter.second->resolve_types(context);
  }

  return_type->resolve_types(context);
}

string FunctionProtoStatement::to_string() const {
  ostringstream oss;
  oss << name->to_string() << "(";

  for (size_t i = 0; i < parameters.size(); ++i) {
    oss << parameters[i].first->to_string() << ": "
        << parameters[i].second->to_string();
    if (i < parameters.size() - 1) {
      oss << ", ";
    }
  }

  oss << ") -> " << return_type->to_string();
  return oss.str();
}

string FunctionProtoStatement::to_string_tree() const {
  ostringstream oss;
  oss << "ProtoStatement(name: " << name->to_string_tree() << ", parameters: [";

  for (size_t i = 0; i < parameters.size(); ++i) {
    oss << "(value: " << parameters[i].first->to_string_tree() << ", type: "
        << (parameters[i].second ? parameters[i].second->to_string_tree() : "")
        << ")";
    if (i < parameters.size() - 1) {
      oss << ", ";
    }
  }

  oss << "], return_type: " << return_type->to_string_tree() << ")";
  return oss.str();
}

void VarProtoStatement::validate(const shared_ptr<ProgramContext> &context) {
  type->validate(context);
}

void VarProtoStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  type->resolve_types(context);
}

string VarProtoStatement::to_string() const {
  return name->to_string() + ": " + type->to_string();
}

string VarProtoStatement::to_string_tree() const {
  return "VarProtoStatement(name: " + name->to_string_tree() +
         ", type: " + type->to_string_tree() + ")";
}