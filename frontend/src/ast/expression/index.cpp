#include "ast/expression/index.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void IndexExpression::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);
  index->validate(context);

  if (!TypeHelper::is_int(index->get_type())) {
    Logger::error("Expected integer as index", LogTypes::Error::TYPE_MISMATCH,
                  index->get_position());
    return;
  }
}

void IndexExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);
  index->resolve_types(context);

  if (auto array_type = TypeHelper::is_array(expression->get_type())) {
    this->array_type = array_type;

    set_type(array_type->child_type);
  } else {
    Logger::error("Array expected", LogTypes::Error::TYPE_MISMATCH,
                  expression->get_position());
  }
}

string IndexExpression::to_string() const {
  return expression->to_string() + "[" + index->to_string() + "]";
}

string IndexExpression::to_string_tree() const {
  return "IndexExpression(type: " + (type ? type->to_string_tree() : "") +
         ", expression: " + expression->to_string_tree() +
         ", index: " + index->to_string_tree() + ")";
}
