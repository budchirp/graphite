#include "ast/expression/integer.hpp"

#include <memory>
#include <string>

void IntegerExpression::validate(const shared_ptr<ProgramContext> &context) {}
void IntegerExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}
void IntegerExpression::resolve_types(
    const shared_ptr<ProgramContext> &context,
    const shared_ptr<IntType> &destination_type) {
  set_type(destination_type ? destination_type
                            : make_shared<IntType>(32, false));
}

string IntegerExpression::to_string() const { return value; }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(type: " + (type ? type->to_string_tree() : "") +
         ", value: '" + value + "')";
}
