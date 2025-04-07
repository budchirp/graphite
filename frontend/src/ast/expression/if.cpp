#include "ast/expression/if.hpp"

#include <memory>
#include <sstream>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"

using namespace std;

void IfExpression::validate(const shared_ptr<ProgramContext> &context) {
  if (!TypeHelper::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on if condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  condition->validate(context);

  consequence->validate(context);
  if (alternative) alternative->validate(context);
}

void IfExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  condition->resolve_types(context);

  consequence->resolve_types(context);
  if (alternative) alternative->resolve_types(context);

  set_type(consequence->get_type());
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
  result << "IfExpression(type: " + type->to_string_tree() + ", condition: "
         << condition->to_string_tree()
         << ", consequence: " << consequence->to_string_tree()
         << ", alternative: "
         << (alternative ? alternative->to_string_tree() : "empty") << ")";
  return result.str();
}
