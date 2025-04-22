#include "ast/expression/unary.hpp"

#include <cmath>
#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"
#include "types/int.hpp"

void UnaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  var_ref->validate(context);

  auto scope = context->get_env()->get_current_scope();

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      if (!TypeHelper::compare(var_ref->get_type(),
                               make_shared<BooleanType>())) {
        Logger::error("Bang operator only supported with booleans",
                      LogTypes::Error::TYPE_MISMATCH, &position);
      }

      break;
    }

    case TOKEN_MINUSMINUS:
    case TOKEN_PLUSPLUS:
    case TOKEN_MINUS:
    case TOKEN_PLUS: {
      auto variable = scope->get_variable(var_ref->name);
      if (!variable->is_mutable) {
        Logger::error("Cannot mutate an immutable variable",
                      LogTypes::Error::SYNTAX, &position);
        return;
      }

      if (!TypeHelper::compare(var_ref->get_type(),
                               make_shared<IntType>(32, false))) {
        Logger::error(
            op.literal + " operator only supported with integer or floats",
            LogTypes::Error::TYPE_MISMATCH, &position);
      }

      break;
    }

    default: {
      Logger::error("Unsupported operator in unary expression",
                    LogTypes::Error::SYNTAX, &position);
    }
  }
}

void UnaryExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  var_ref->resolve_types(context);

  auto variable_type = var_ref->get_type();
  switch (op.type) {
    case TOKEN_BANG_BANG: {
      if (auto null_type = TypeHelper::is_null(variable_type)) {
        set_type(null_type->child_type);
      } else {
        Logger::error("Trying to make a non-null type non-null",
                      LogTypes::Error::TYPE_MISMATCH, var_ref->get_position());
        return;
      }

      break;
    }

    case TOKEN_ASTERISK: {
      if (auto pointer_type = TypeHelper::is_pointer(variable_type)) {
        set_type(pointer_type->pointee_type);
      } else {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH, var_ref->get_position());
        return;
      }

      break;
    }

    case TOKEN_AMPERSAND: {
      set_type(make_shared<PointerType>(variable_type, false));
      break;
    }

    default:
      set_type(variable_type);
      break;
  }
}

string UnaryExpression::to_string() const {
  return op.to_string() + var_ref->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "UnaryExpression(type: " + (type ? type->to_string_tree() : "") + ", op: '" +
         op.to_string_tree() + "', right: " + var_ref->to_string_tree() + ")";
}
