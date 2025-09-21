#include "ast/expression/array.hpp"

#include <cstddef>
#include <memory>
#include <string>

#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/null.hpp"

void ArrayExpression::validate(const shared_ptr<ProgramContext> &context) {
  for (const auto &value : values) {
    value->validate(context);

    if (!TypeHelper::compare(type->child_type, value->get_type())) {
      Logger::type_error("Type mismatch", value->get_position(), type,
                         value->get_type());
      return;
    }
  }
}
void ArrayExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}
void ArrayExpression::resolve_types(
    const shared_ptr<ProgramContext> &context,
    const shared_ptr<ArrayType> &destination_type) {
  for (const auto &value : values) {
    value->resolve_types(context);
  }

  set_type(destination_type
               ? destination_type
               : make_shared<ArrayType>(values.size() > 0
                                            ? values[0]->get_type()
                                            : make_shared<NullType>(nullptr),
                                        values.size()));
}

string ArrayExpression::to_string() const {
  ostringstream result;
  result << "[";

  for (size_t i = 0; i < values.size(); ++i) {
    result << values[i]->to_string();
    if (i < values.size() - 1) {
      result << ", ";
    }
  }

  result << "]";
  return result.str();
}

string ArrayExpression::to_string_tree() const {
  ostringstream result;
  result << "ArrayExpression(type: " << (type ? type->to_string_tree() : "")
         << ", values: [";

  for (size_t i = 0; i < values.size(); ++i) {
    result << values[i]->to_string_tree();
    if (i < values.size() - 1) {
      result << ", ";
    }
  }

  result << "])";
  return result.str();
}
