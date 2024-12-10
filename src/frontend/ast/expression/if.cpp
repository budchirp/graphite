#include <memory>
#include <sstream>
#include <string>

#include "frontend/ast/expression/if.hpp"

using namespace std;

IfExpression::IfExpression(unique_ptr<Expression> condition,
                           unique_ptr<BlockStatement> consequence,
                           unique_ptr<BlockStatement> alternative) {
  this->condition = std::move(condition);
  this->consequence = std::move(consequence);
  this->alternative = std::move(alternative);
}

string IfExpression::to_string() const {
  ostringstream result;
  result << "if (" << condition->to_string() << ") "
         << consequence->to_string();

  if (alternative) {
    result << " else " << alternative->to_string();
  }

  return result.str();
}

string IfExpression::to_string_tree() const {
  ostringstream result;
  result << "IfExpression(condition: " << condition->to_string_tree()
         << ", consequence: " << consequence->to_string_tree()
         << ", alternative: " << alternative->to_string_tree() << ")";
  return result.str();
}
