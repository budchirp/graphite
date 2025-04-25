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
  identifier->validate(context);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(identifier->value);

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      if (!TypeHelper::compare(variable->type, make_shared<BooleanType>())) {
        Logger::error("Bang operator only supported with booleans",
                      LogTypes::Error::TYPE_MISMATCH, &position);
      }

      break;
    }

    case TOKEN_MINUSMINUS:
    case TOKEN_PLUSPLUS:
    case TOKEN_MINUS:
    case TOKEN_PLUS: {
      if (!variable->is_mutable) {
        Logger::error("Cannot mutate an immutable variable",
                      LogTypes::Error::SYNTAX, &position);
        return;
      }

      auto int_type = make_shared<IntType>(32, false);
      if (!TypeHelper::compare(variable->type, int_type)) {
        Logger::type_error(
            op.literal + " operator can only be used with integer or floats",
            &position, variable->type, int_type);
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
  identifier->resolve_types(context);

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(identifier->value);

  switch (op.type) {
    case TOKEN_BANG_BANG: {
      if (auto null_type = TypeHelper::is_null(variable->type)) {
        set_type(null_type->child_type);
      } else {
        Logger::error("Trying to make a non-null type non-null",
                      LogTypes::Error::TYPE_MISMATCH,
                      identifier->get_position());
        return;
      }

      break;
    }

    case TOKEN_ASTERISK: {
      if (auto pointer_type = TypeHelper::is_pointer(variable->type)) {
        set_type(pointer_type->pointee_type);
      } else {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH,
                      identifier->get_position());
        return;
      }

      break;
    }

    case TOKEN_AMPERSAND: {
      set_type(make_shared<PointerType>(variable->type, false));
      break;
    }

    default:
      set_type(variable->type);
      break;
  }
}

string UnaryExpression::to_string() const {
  return op.to_string() + identifier->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "UnaryExpression(type: " + (type ? type->to_string_tree() : "") +
         ", op: '" + op.to_string_tree() +
         "', expression: " + identifier->to_string_tree() + ")";
}
