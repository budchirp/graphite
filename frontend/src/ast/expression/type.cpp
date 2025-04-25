#include "ast/expression/type.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/parser/unknown_parser_type.hpp"

void TypeExpression::validate(const shared_ptr<ProgramContext> &context) {
  // if (!context->get_env()->get_type(type->get_name())) {
  //   Logger::error("Undefined type `" + type->to_string() + "`",
  //                 LogTypes::Error::UNDEFINED, &position);
  // }
}

void TypeExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  set_type(resolve_types(context, type));
}

shared_ptr<Type> TypeExpression::resolve_types(
    const shared_ptr<ProgramContext> &context,
    const shared_ptr<Type> &type) const {
  shared_ptr<Type> parsed_type;
  if (auto pointer_type = TypeHelper::is_pointer(type)) {
    parsed_type = make_shared<PointerType>(
        resolve_types(context, pointer_type->pointee_type),
        pointer_type->is_mutable);
  } else if (auto null_type = TypeHelper::is_null(type)) {
    parsed_type =
        make_shared<NullType>(resolve_types(context, null_type->child_type));
  } else if (auto array_type = TypeHelper::is_array(type)) {
    parsed_type = make_shared<ArrayType>(
        resolve_types(context, array_type->child_type), array_type->size);
  } else {
    parsed_type = context->get_env()->get_type(
        dynamic_pointer_cast<UnknownParserType>(type)->value);
  }

  if (!parsed_type) {
    Logger::error("Failed to parse type", LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  return parsed_type;
}

string TypeExpression::to_string() const { return type->to_string(); }

string TypeExpression::to_string_tree() const {
  return "TypeExpression(type: " + (type ? type->to_string_tree() : "") + ")";
}
