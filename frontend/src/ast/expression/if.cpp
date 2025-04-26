#include "ast/expression/if.hpp"

#include <memory>
#include <sstream>
#include <string>

#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"

using namespace std;

void IfExpression::validate(const shared_ptr<ProgramContext> &context) {
  condition->validate(context);

  auto boolean_type = make_shared<BooleanType>();
  if (!TypeHelper::compare(condition->get_type(), boolean_type)) {
    Logger::type_error("Only booleans are allowed on if condition",
                       condition->get_position(), condition->get_type(),
                       boolean_type);
    return;
  }

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
  result << "IfExpression(type: " + (type ? type->to_string_tree() : "") +
                ", condition: "
         << condition->to_string_tree()
         << ", consequence: " << consequence->to_string_tree()
         << ", alternative: "
         << (alternative ? alternative->to_string_tree() : "empty") << ")";
  return result.str();
}
