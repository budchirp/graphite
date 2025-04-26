#include "ast/expression/call.hpp"

#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

using namespace std;

void CallExpression::validate(const shared_ptr<ProgramContext> &context) {
  auto function = context->get_env()->get_function(name->value)->type;
  if (function->parameters.size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function `" +
                      name->value + "`",
                  LogTypes::Error::SYNTAX, name->get_position());
    return;
  }

  size_t idx = 0;
  for (const auto &argument : arguments) {
    argument->validate(context);

    auto parameter = function->parameters[idx++];
    if (!TypeHelper::compare(parameter.second, argument->get_type())) {
      Logger::type_error("Type mismatch in argument `" + parameter.first + "`",
                         argument->get_position(), parameter.second,
                         argument->get_type());
      return;
    }
  }
}

void CallExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  auto function = context->get_env()->get_function(name->value);
  if (!function) {
    Logger::error("Unknown function `" + name->to_string() + "` called",
                  LogTypes::Error::UNDEFINED, name->get_position());
    return;
  }

  for (const auto &argument : arguments) {
    argument->resolve_types(context);
  }

  set_type(function->type->return_type);
}

string CallExpression::to_string() const {
  ostringstream result;
  result << name->to_string() << "(";

  for (size_t i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << ")";
  return result.str();
}

string CallExpression::to_string_tree() const {
  ostringstream result;
  result << "CallExpression(type: " << (type ? type->to_string_tree() : "")
         << ", name: " << name->to_string_tree() << ", arguments: [";

  for (size_t i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << "])";
  return result.str();
}
