
#include "ast/expression/field.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void FieldExpression::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);
  field->validate(context);
}

void FieldExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);
  field->resolve_types(context);

  if (auto struct_type = TypeHelper::is_struct(expression->get_type())) {
    this->struct_type = struct_type;

    auto it = struct_type->fields.find(field->value);
    if (it != struct_type->fields.end()) {
      set_type(it->second);
    } else {
      Logger::error("Field `" + field->to_string() +
                        "` does not exist on struct `" + expression->to_string() +
                        "`",
                    LogTypes::Error::INTERNAL, field->get_position());
    }
  } else {
    Logger::error("Struct expected", LogTypes::Error::SYNTAX,
                  expression->get_position());
    return;
  }
}

string FieldExpression::to_string() const {
  return expression->to_string() + "." + field->to_string();
}

string FieldExpression::to_string_tree() const {
  return "FieldExpression(type: " + (type ? type->to_string_tree() : "") +
         ", expression: " + expression->to_string_tree() +
         ", field: " + field->to_string_tree() + ")";
}
