#include "ast/expression/struct.hpp"

#include <cstddef>
#include <memory>
#include <sstream>

#include "types/struct.hpp"

void StructExpression::validate(const shared_ptr<ProgramContext> &context) {
  for (const auto &[_, field_value] : fields) {
    field_value->validate(context);
  }
}

void StructExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}

void StructExpression::resolve_types(const shared_ptr<ProgramContext> &context,
                                     const shared_ptr<Type> &destination_type) {
  // TODO: DONT FORGET CHECKS

  vector<pair<string, shared_ptr<Type>>> fields_type;
  fields_type.reserve(fields.size());
  for (const auto &[field_name, field_value] : fields) {
    field_value->resolve_types(context);

    fields_type.emplace_back(field_name->value, field_value->get_type());
  }

  set_type(destination_type ? dynamic_pointer_cast<StructType>(destination_type)
                            : make_shared<StructType>(fields_type));
}

string StructExpression::to_string() const {
  ostringstream oss;
  oss << name->to_string() << " {\n";

  for (size_t i = 0; i < fields.size(); ++i) {
    oss << "  " << fields[i].first->to_string() << ": "
        << fields[i].second->to_string();
    if (i < fields.size() - 1) {
      oss << ",\n";
    }
  }

  oss << "}";
  return oss.str();
}

string StructExpression::to_string_tree() const {
  ostringstream oss;
  oss << "StructExpression(name: " << name->to_string_tree() << ", fields: {";

  for (size_t i = 0; i < fields.size(); ++i) {
    oss << "'" << fields[i].first->to_string() << "': ["
        << fields[i].first->to_string_tree() << ", "
        << fields[i].second->to_string_tree() << "]";
    if (i < fields.size() - 1) {
      oss << ",\n";
    }
  }

  oss << "}";

  return oss.str();
}
