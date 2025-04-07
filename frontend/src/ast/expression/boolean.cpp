#include "ast/expression/boolean.hpp"

#include <memory>
#include <string>

#include "types/boolean.hpp"

void BooleanExpression::validate(const shared_ptr<ProgramContext> &context) {}
void BooleanExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  set_type(make_shared<BooleanType>());
}

string BooleanExpression::to_string() const { return ::to_string(value); }

string BooleanExpression::to_string_tree() const {
  return "BooleanExpression(type: " + type->to_string_tree() + ", value: '" +
         ::to_string(value) + "')";
}
