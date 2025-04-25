#include "ast/expression/struct.hpp"

#include <cstddef>
#include <memory>
#include <sstream>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/struct.hpp"

void StructExpression::validate(const shared_ptr<ProgramContext> &context) {
  auto struct_type = dynamic_pointer_cast<StructType>(
      context->get_env()->get_type(name->value));

  for (const auto &[_, field] : fields) {
    field.second->validate(context);

    auto it = struct_type->fields.find(field.first->value);
    if (it == struct_type->fields.end()) {
      Logger::error("Unknown field `" + field.first->value + "`",
                    LogTypes::Error::SYNTAX, field.first->get_position());
      return;
    }
  }

  for (const auto &[field_name, field_type] : struct_type->fields) {
    auto it = fields.find(field_name);
    if (it == fields.end()) {
      Logger::error("`" + field_name + "` is uninitialized",
                    LogTypes::Error::SYNTAX, &position);
      return;
    }

    auto field = it->second;

    if (!TypeHelper::compare(field_type, field.second->get_type())) {
      Logger::type_error("Type mismatch in field `" + field.first->value + "`",
                         &position, field_type, field.second->get_type());
    }
  }
}

void StructExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}

void StructExpression::resolve_types(const shared_ptr<ProgramContext> &context,
                                     const shared_ptr<Type> &destination_type) {
  auto struct_type = dynamic_pointer_cast<StructType>(
      context->get_env()->get_type(name->value));
  if (!struct_type) {
    Logger::error("Unknown struct `" + name->to_string() + "`");
  }

  for (const auto &[_, field] : fields) {
    field.second->resolve_types(context);
  }

  set_type(destination_type ? dynamic_pointer_cast<StructType>(destination_type)
                            : struct_type);
}

string StructExpression::to_string() const {
  ostringstream oss;
  oss << name->to_string() << " {\n";

  size_t i = 0;
  for (const auto &[_, value] : fields) {
    const auto [field_name, field_value] = value;

    oss << "  " << field_name->to_string() << ": " << field_value->to_string();
    if (i++ < fields.size() - 1) {
      oss << ",\n";
    }
  }

  oss << "}";
  return oss.str();
}

string StructExpression::to_string_tree() const {
  ostringstream oss;
  oss << "StructExpression(name: " << name->to_string_tree() << ", fields: {";

  size_t i = 0;
  for (const auto &[_, value] : fields) {
    const auto [field_name, field_value] = value;

    oss << "'" << field_name->to_string() << "': ["
        << field_name->to_string_tree() << ", " << field_value->to_string_tree()
        << "]";
    if (i++ < fields.size() - 1) {
      oss << ",\n";
    }
  }

  oss << "}";

  return oss.str();
}
