#include "ast/expression/string.hpp"

#include <regex>
#include <string>

void StringExpression::validate(const shared_ptr<ProgramContext> &context) {}
void StringExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  set_type(make_shared<StringType>());
}

string StringExpression::to_string() const {
  return "\"" + regex_replace(value, regex("\n"), "\\n") + "\"";
}

string StringExpression::to_string_tree() const {
  return "StringExpression(type: " + (type ? type->to_string_tree() : "") + ", value: '" +
         value + "')";
}
