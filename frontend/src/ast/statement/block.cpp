#include "ast/statement/block.hpp"

#include <cstddef>
#include <sstream>

void BlockStatement::validate(const shared_ptr<ProgramContext> &context) {
  for (const auto &statement : statements) {
    statement->validate(context);
  }
}

void BlockStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  for (const auto &statement : statements) {
    statement->resolve_types(context);

    type = statement->get_type();
  }
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
  oss << "BlockStatement(type: " << (type ? type->to_string_tree() : "") << ", statement: [";

  for (size_t i = 0; i < statements.size(); ++i) {
    oss << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      oss << ", ";
    }
  }

  oss << "]";

  return oss.str();
}
