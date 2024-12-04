#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "frontend/ast/expressions/call.hpp"
#include "std/types.hpp"

using namespace std;

CallExpression::CallExpression(unique_ptr<IdentifierExpression> name,
                               vector<unique_ptr<Expression>> arguments) {
  this->name = std::move(name);
  this->arguments = std::move(arguments);
}

string CallExpression::to_string() const {
  ostringstream result;
  result << name->to_string() << "(";

  for (i32 i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << ")";

  return result.str();
}

string CallExpression::to_string_tree() const {
  ostringstream result;
  result << "CallExpression(name: " << name->to_string_tree()
         << ", parameters: [";

  for (i32 i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << "])";

  return result.str();
}
