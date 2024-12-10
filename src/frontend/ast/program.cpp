#include <sstream>
#include <vector>

#include "frontend/ast/program.hpp"
#include "frontend/ast/statement.hpp"
#include "std/types.hpp"

Program::Program(vector<unique_ptr<Statement>> statements) {
  this->statements = std::move(statements);
}

string Program::to_string() const {
  ostringstream result;

  for (auto i = 0; i < statements.size(); ++i) {
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

  for (auto i = 0; i < statements.size(); ++i) {
    result << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      result << ", ";
    }
  }

  result << "])";

  return result.str();
}
