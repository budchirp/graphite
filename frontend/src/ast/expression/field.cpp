
#include "ast/expression/field.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void FieldExpression::validate(const shared_ptr<ProgramContext> &context) {
  identifier->validate(context);
  field->validate(context);
}

void FieldExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  identifier->resolve_types(context);
  field->resolve_types(context);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(identifier->value);

  if (auto struct_type = TypeHelper::is_struct(variable->type)) {
    this->struct_type = struct_type;

    auto it = struct_type->fields.find(field->value);
    if (it != struct_type->fields.end()) {
      set_type(it->second);
    } else {
      Logger::error("Field `" + field->to_string() + "` does not exist on struct `" +
                        identifier->to_string() + "`",
                    LogTypes::Error::INTERNAL, field->get_position());
    }
  } else {
    Logger::error("Struct expected", LogTypes::Error::SYNTAX,
                  identifier->get_position());
    return;
  }
}

string FieldExpression::to_string() const {
  return identifier->to_string() + "[" + field->to_string() + "]";
}

string FieldExpression::to_string_tree() const {
  return "FieldExpression(type: " + (type ? type->to_string_tree() : "") +
         ", identifier: " + identifier->to_string_tree() +
         ", field: " + field->to_string_tree() + ")";
}
