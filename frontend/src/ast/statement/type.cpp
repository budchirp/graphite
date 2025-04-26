#include "ast/statement/type.hpp"

#include <memory>
#include <sstream>

#include "semantic/symbols/type.hpp"

void TypeStatement::validate(const shared_ptr<ProgramContext> &context) {
  type->validate(context);
}

void TypeStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  type->resolve_types(context);

  context->get_env()->add_type(
      name->value,
      make_shared<TypeSymbol>(name->value, visibility, type->get_type()));
}

string TypeStatement::to_string() const {
  ostringstream oss;
  oss << "type " << name->to_string() << " = " << type->to_string();
  return oss.str();
}

string TypeStatement::to_string_tree() const {
  ostringstream oss;
  oss << "TypeStatement(name: " << name->to_string_tree()
      << ", type: " << type->to_string_tree() << ")";
  return oss.str();
}
