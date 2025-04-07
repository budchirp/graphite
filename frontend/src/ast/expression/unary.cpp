#include "ast/expression/unary.hpp"

#include <cmath>
#include <memory>
#include <string>

#include "ast/expression/var_ref.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"
#include "types/int.hpp"

void UnaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);

  auto scope = context->get_env()->get_current_scope();

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      if (!TypeHelper::compare(expression->get_type(),
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
      auto variable = scope->get_variable(expression->name);
      if (!variable->is_mutable) {
        Logger::error("Cannot mutate an immutable variable",
                      LogTypes::Error::SYNTAX, &position);
        return;
      }

      if (!TypeHelper::compare(expression->get_type(),
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
  expression->resolve_types(context);

  auto expression_type = expression->get_type();

  shared_ptr<Type> type;
  switch (op.type) {
    case TOKEN_BANG_BANG: {
      if (auto null_type = TypeHelper::is_null(expression_type)) {
        type = null_type->child_type;
      } else {
        Logger::error("Trying to make a non-null type non-null",
                      LogTypes::Error::TYPE_MISMATCH,
                      expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_ASTERISK: {
      if (auto pointer_type = TypeHelper::is_pointer(expression_type)) {
        type = pointer_type->pointee_type;
      } else {
        Logger::error("Cannot dereference non-pointer type",
                      LogTypes::Error::TYPE_MISMATCH,
                      expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_AMPERSAND: {
      type = make_shared<PointerType>(expression_type);
      break;
    }

    default:
      type = expression_type;
      break;
  }

  set_type(type);
}

string UnaryExpression::to_string() const {
  return op.to_string() + expression->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "PrefixExpression(type: " + type->to_string_tree() + ", op: '" +
         op.to_string_tree() + "', right: " + expression->to_string_tree() +
         ")";
}
