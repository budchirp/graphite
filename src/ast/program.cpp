#include <llvm/IR/Value.h>
#include <sstream>
#include <vector>

#include "ast/statement.hpp"
#include "ast/program.hpp"

using namespace llvm;

Value *Program::codegen() const {
  for (const auto &statement : statements) {
    statement->codegen();
  }

  return nullptr;
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
