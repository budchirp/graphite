#include <memory>

#include "frontend/ast/statement/extern.hpp"
#include "frontend/ast/statement/proto.hpp"

ExternStatement::ExternStatement(const Position &position, unique_ptr<ProtoStatement> proto) 
    : position(position) {
  this->proto = std::move(proto);
}

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree()+ ")";
}
