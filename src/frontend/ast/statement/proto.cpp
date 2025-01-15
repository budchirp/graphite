#include <memory>
#include <sstream>

#include "frontend/ast/statement/proto.hpp"

ProtoStatement::ProtoStatement(const Position &position, unique_ptr<IdentifierExpression> name,
                               vector<pair<unique_ptr<IdentifierExpression>,
                                           unique_ptr<IdentifierExpression>>>
                                   parameters,
                               unique_ptr<IdentifierExpression> return_type) 
    : position(position) {
  this->name = std::move(name);
  this->parameters = std::move(parameters);
  this->return_type = std::move(return_type);
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
