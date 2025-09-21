#include "ast/expression/unary.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/int.hpp"

void UnaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);

  switch (op.type) {
    case TOKEN_ASTERISK:
    case TOKEN_AMPERSAND: {
      break;
    }

    case TOKEN_BANG: {
      auto boolean_type = make_shared<BooleanType>();
      if (!TypeHelper::compare(expression->get_type(),boolean_type )) {
        Logger::type_error("Bang operator only supported with booleans",
                      &position, boolean_type, expression->get_type());
      }

      break;
    }

    case TOKEN_MINUSMINUS:
    case TOKEN_PLUSPLUS:
    case TOKEN_MINUS:
    case TOKEN_PLUS: {
      auto int_type = make_shared<IntType>(32, false);
      if (!TypeHelper::compare(expression->get_type(), int_type)) {
        Logger::type_error(
            op.literal + " operator can only be used with integers",
            &position, int_type, expression->get_type());
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

  auto type = expression->get_type();

  switch (op.type) {
    case TOKEN_BANG_BANG: {
      if (auto null_type = TypeHelper::is_null(type)) {
        set_type(null_type->child_type);
      } else {
        Logger::warn("Trying to make a non-null type non-null",
                      LogTypes::Warn::SUGGESTION,
                      expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_ASTERISK: {
      cout << type->to_string_tree() << endl;
      if (auto pointer_type = TypeHelper::is_pointer(type)) {
        set_type(pointer_type->pointee_type);
      } else {
        Logger::error("Cannot dereference non-pointer type.",
                      LogTypes::Error::TYPE_MISMATCH,
                      expression->get_position());
        return;
      }

      break;
    }

    case TOKEN_AMPERSAND: {
      set_type(make_shared<PointerType>(type, false));
      break;
    }

    default:
      set_type(type);
      break;
  }
}

string UnaryExpression::to_string() const {
  return op.to_string() + type->to_string();
}

string UnaryExpression::to_string_tree() const {
  return "UnaryExpression(type: " + (type ? type->to_string_tree() : "") +
         ", op: '" + op.to_string_tree() +
         "', expression: " + expression->to_string_tree() + ")";
}
