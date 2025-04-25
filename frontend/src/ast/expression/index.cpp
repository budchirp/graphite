#include "ast/expression/index.hpp"

#include <memory>
#include <string>

#include "ast/expression/string.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void IndexExpression::validate(const shared_ptr<ProgramContext> &context) {
  identifier->validate(context);
  index->validate(context);
}

void IndexExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  identifier->resolve_types(context);
  index->resolve_types(context);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(identifier->value);

  if (auto array_type = TypeHelper::is_array(variable->type)) {
    if (!TypeHelper::is_int(index->get_type())) {
      Logger::error("Expected integer as index", LogTypes::Error::TYPE_MISMATCH,
                    index->get_position());
      return;
    }

    set_type(array_type->child_type);
  } else if (auto struct_type = TypeHelper::is_struct(variable->type)) {
    if (!TypeHelper::is_string(index->get_type())) {
      Logger::error("Expected string as index", LogTypes::Error::TYPE_MISMATCH,
                    index->get_position());
      return;
    }

    // FIX: THIS SHIT
    auto field = dynamic_pointer_cast<StringExpression>(index)->value;

    shared_ptr<Type> field_type;
    for (const auto &[type_field_name, type_field_type] : struct_type->fields) {
      if (type_field_name == field) {
        field_type = type_field_type;
        break;
      }
    }

    if (!field_type) {
      Logger::error("Field `" + field + "` does not exist on struct `" +
                        identifier->value + "`",
                    LogTypes::Error::INTERNAL, index->get_position());
    }

    set_type(field_type);
  } else {
    Logger::error("Array or struct expected", LogTypes::Error::TYPE_MISMATCH,
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
