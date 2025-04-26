#include "ast/expression/var_ref.hpp"

void VarRefExpression::validate(const shared_ptr<ProgramContext> &context) {}

void VarRefExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  set_type(context->get_env()->get_current_scope()->get_variable(name)->type);
}

string VarRefExpression::to_string() const { return name; }

string VarRefExpression::to_string_tree() const {
  return "VarRefExpression(type: " + (type ? type->to_string_tree() : "") +
         ", name: '" + name + "')";
}
