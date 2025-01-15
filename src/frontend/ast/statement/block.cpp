#include <sstream>

#include "frontend/ast/statement/block.hpp"

BlockStatement::BlockStatement(const Position &position, vector<unique_ptr<Statement>> statements) 
    : position(position) {
  this->statements = std::move(statements);
}

string BlockStatement::to_string() const {
  ostringstream oss;
  oss << "{\n";

  for (const auto &statement : statements) {
    oss << "  " << statement->to_string() << ";\n";
  }

  oss << "}";
  return oss.str();
}

string BlockStatement::to_string_tree() const {
  ostringstream oss;
  oss << "BlockStatement(statement: [";

  for (auto i = 0; i < statements.size(); ++i) {
    oss << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      oss << ", ";
    }
  }

  oss << "]";

  return oss.str();
}
