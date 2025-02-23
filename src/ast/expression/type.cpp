#include "ast/expression/type.hpp"

#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/parser/unknown_parser_type.hpp"

llvm::Value *TypeExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return nullptr;
}

void TypeExpression::validate(const shared_ptr<ProgramContext> &context) {
  if (!context->get_env()->get_type(type->get_name())) {
    Logger::error("Undefined type `" + type->to_string() + "`",
                  LogTypes::Error::UNDEFINED, &position);
  }
}

void TypeExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  set_type(resolve_types(context, type));
}

shared_ptr<Type> TypeExpression::resolve_types(const shared_ptr<ProgramContext> &context, const shared_ptr<Type> &type) const {
  shared_ptr<Type> _type;
  if (auto pointer_type = TypeHelper::is_pointer(type)) {
    _type = make_shared<PointerType>(resolve_types(context, pointer_type->pointee_type));
  } else if (auto null_type = TypeHelper::is_null(type)) {
    _type = make_shared<NullType>(resolve_types(context, null_type->child_type));
  } else if (auto array_type = TypeHelper::is_array(type)) {
    _type = make_shared<ArrayType>(resolve_types(context, array_type->child_type), array_type->size);
  } else {
    _type = context->get_env()->get_type(dynamic_pointer_cast<UnknownParserType>(type)->value);
  }

  if (!_type) {
    Logger::error("Failed to parse type", LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  return _type;
}

string TypeExpression::to_string() const { return type->to_string(); }

string TypeExpression::to_string_tree() const {
  return "TypeExpression(type: " + type->to_string_tree() + ")";
}
