#include <sstream>

#include "frontend/ast/statements/block.hpp"
#include "std/types.hpp"

BlockStatement::BlockStatement(vector<unique_ptr<Statement>> statements) {
  this->statements = std::move(statements);
}

std::string BlockStatement::to_string() const {
  std::ostringstream oss;
  oss << "{\n";

  for (const auto &statement : statements) {
    oss << "  " << statement->to_string() << ";\n";
  }

  oss << "}";
  return oss.str();
}

std::string BlockStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "BlockStatement(statements: [";

  for (i32 i = 0; i < statements.size(); ++i) {
    oss << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      oss << ", ";
    }
  }

  oss << "]";

  return oss.str();
}
