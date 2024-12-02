#include <memory>
#include <sstream>

#include "frontend/ast/statements/function.hpp"

FunctionStatement::FunctionStatement(unique_ptr<ProtoStatement> proto,
                                     unique_ptr<BlockStatement> body) {
  this->proto = std::move(proto);
  this->body = std::move(body);
}

std::string FunctionStatement::to_string() const {
  std::ostringstream oss;
  oss << "fn " << proto->to_string() << " " << body->to_string();
  return oss.str();
}

std::string FunctionStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "FunctionStatement(proto: " << proto->to_string_tree()
      << ", body: " << body->to_string_tree() << ")";
  return oss.str();
}
