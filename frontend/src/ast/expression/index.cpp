#include "ast/expression/index.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void IndexExpression::validate(const shared_ptr<ProgramContext> &context) {
  identifier->validate(context);
  index->validate(context);

  if (!TypeHelper::is_int(index->get_type())) {
    Logger::error("Expected integer as index", LogTypes::Error::TYPE_MISMATCH,
                  index->get_position());
    return;
  }
}

void IndexExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  identifier->resolve_types(context);
  index->resolve_types(context);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(identifier->value);

  if (auto array_type = TypeHelper::is_array(variable->type)) {
    this->array_type = array_type;

    set_type(array_type->child_type);
  } else {
    Logger::error("Array expected", LogTypes::Error::TYPE_MISMATCH,
                  identifier->get_position());
  }
}

string IndexExpression::to_string() const {
  return identifier->to_string() + "[" + index->to_string() + "]";
}

string IndexExpression::to_string_tree() const {
  return "IndexExpression(type: " + (type ? type->to_string_tree() : "") +
         ", identifier: " + identifier->to_string_tree() +
         ", index: " + index->to_string_tree() + ")";
}
