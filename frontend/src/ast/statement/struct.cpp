#include "ast/statement/struct.hpp"

#include <cstddef>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "semantic/symbols/type.hpp"
#include "types/struct.hpp"

void StructStatement::validate(const shared_ptr<ProgramContext> &context) {
  for (const auto &[field_name, field_type] : fields) {
    field_type->validate(context);
  }
}

void StructStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  unordered_map<string, shared_ptr<Type>> fields_type;
  fields_type.reserve(fields.size());
  for (const auto &[field_name, field_type] : fields) {
    field_type->resolve_types(context);

    fields_type.insert_or_assign(field_name->value, field_type->get_type());
  }

  context->get_env()->add_type(
      name->value,
      make_shared<TypeSymbol>(name->value, visibility,
                              make_shared<StructType>(fields_type)));
}

string StructStatement::to_string() const {
  ostringstream oss;
  oss << "struct " << name->to_string() << " {\n";

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

string StructStatement::to_string_tree() const {
  ostringstream oss;
  oss << "StructStatement(name: " << name->to_string_tree() << ", fields: {";

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
