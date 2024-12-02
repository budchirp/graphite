#include <memory>
#include <sstream>

#include "frontend/ast/statements/proto.hpp"
#include "std/types.hpp"

ProtoStatement::ProtoStatement(
    unique_ptr<IdentifierExpression> name,
    vector<unique_ptr<IdentifierExpression>> arguments) {
  this->name = std::move(name);
  this->arguments = std::move(arguments);
}

std::string ProtoStatement::to_string() const {
  std::ostringstream oss;
  oss << name->to_string() << "(";

  for (i32 i = 0; i < arguments.size(); ++i) {
    oss << arguments[i]->to_string();
    if (i < arguments.size() - 1) {
      oss << ", ";
    }
  }

  oss << ")";
  return oss.str();
}

std::string ProtoStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "ProtoStatement(name: " << name->to_string_tree() << ", arguments: [";

  for (i32 i = 0; i < arguments.size(); ++i) {
    oss << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      oss << ", ";
    }
  }

  oss << "])";
  return oss.str();
}
