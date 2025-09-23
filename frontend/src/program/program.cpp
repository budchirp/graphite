#include "program/program.hpp"

#include <sstream>
#include <vector>

void Program::validate() {
  for (const auto &statement : statements) {
    statement->validate(context);
  }
}

void Program::resolve_types() {
  for (const auto &statement : statements) {
    statement->resolve_types(context);
  }
}

string Program::to_string() const {
  ostringstream result;

  for (size_t i = 0; i < statements.size(); ++i) {
    result << statements[i]->to_string();
    if (i < statements.size() - 1) {
      result << "\n";
    }
  }

  return result.str();
}

string Program::to_string_tree() const {
  ostringstream result;
  result << "Program(statements: [";

  for (size_t i = 0; i < statements.size(); ++i) {
    result << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      result << ", ";
    }
  }

  result << "])";

  return result.str();
}
