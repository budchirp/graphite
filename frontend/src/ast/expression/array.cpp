#include "ast/expression/array.hpp"

#include <cstddef>
#include <memory>
#include <string>

#include "types/null.hpp"

void ArrayExpression::validate(const shared_ptr<ProgramContext> &context) {}
void ArrayExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}
void ArrayExpression::resolve_types(const shared_ptr<ProgramContext> &context,
                                    const shared_ptr<Type> &destination_type) {
  for (const auto &value : values) {
    value->resolve_types(context);
  }

  set_type(destination_type ? dynamic_pointer_cast<ArrayType>(destination_type)
                            : make_shared<ArrayType>(
                                  values[0] ? values[0]->get_type()
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
  result << "ArrayExpression(type: " << (type ? type->to_string_tree() : "") << ", values: [";

  for (size_t i = 0; i < values.size(); ++i) {
    result << values[i]->to_string_tree();
    if (i < values.size() - 1) {
      result << ", ";
    }
  }

  result << "])";
  return result.str();
}
