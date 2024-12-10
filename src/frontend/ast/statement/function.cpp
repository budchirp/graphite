#include <memory>
#include <sstream>

#include "frontend/ast/statement/function.hpp"

FunctionStatement::FunctionStatement(unique_ptr<ProtoStatement> proto,
                                     unique_ptr<BlockStatement> body) {
  this->proto = std::move(proto);
  this->body = std::move(body);
}

string FunctionStatement::to_string() const {
  ostringstream oss;
  oss << "fn " << proto->to_string() << " " << body->to_string();
  return oss.str();
}

string FunctionStatement::to_string_tree() const {
  ostringstream oss;
  oss << "FunctionStatement(proto: " << proto->to_string_tree()
      << ", body: " << body->to_string_tree() << ")";
  return oss.str();
}
