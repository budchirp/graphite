#include <memory>
#include <sstream>

#include "frontend/ast/statements/extern.hpp"
#include "frontend/ast/statements/proto.hpp"

ExternStatement::ExternStatement(unique_ptr<ProtoStatement> proto) {
  this->proto = std::move(proto);
}

std::string ExternStatement::to_string() const {
  std::ostringstream oss;
  oss << "extern ";
  oss << proto->to_string();
  return oss.str();
}

std::string ExternStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "ExternStatement(proto: " << proto->to_string_tree() << ")";
  return oss.str();
}
