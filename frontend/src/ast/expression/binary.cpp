#include "ast/expression/binary.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "ast/expression/type.hpp"
#include "ast/expression/unary.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/pointer.hpp"
#include "types/string.hpp"

void BinaryExpression::validate(const shared_ptr<ProgramContext> &context) {
  left->validate(context);
  right->validate(context);

  if (op.type == TOKEN_AS) return;

  auto left_type = left->get_type();
  auto right_type = right->get_type();

  if (op.type != TOKEN_ASSIGN &&
      !TypeHelper::compare(left_type, right_type,
                           op.type == TOKEN_ASSIGN || op.type == TOKEN_EQUAL ||
                               op.type == TOKEN_NOT_EQUAL)) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  switch (op.type) {
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL: {
      return;
    }

    case TOKEN_ASSIGN: {
      if (auto identifier = IdentifierExpression::is_identifier(left)) {
        auto scope = context->get_env()->get_current_scope();
        auto variable = scope->get_variable(identifier->value);

        if (auto unary_expression = dynamic_pointer_cast<UnaryExpression>(left);
            unary_expression && unary_expression->op.type == TOKEN_ASTERISK) {
          if (auto pointer_type = TypeHelper::is_pointer(variable->type); pointer_type) {
            if (!TypeHelper::compare(pointer_type->pointee_type, right_type,
                                     true)) {
              Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                            &position);
              return;
            }

            if (!pointer_type->is_mutable)
              Logger::error("Cannot mutate an immutable pointer variable",
                            LogTypes::Error::SYNTAX, right->get_position());
          }
        } else {
          if (!TypeHelper::compare(variable->type, right_type, true)) {
            Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                          &position);
            return;
          }

          if (!(variable->is_mutable || !variable->is_initialized)) {
            Logger::error("Cannot mutate an immutable variable",
                          LogTypes::Error::SYNTAX, right->get_position());
          }
        }
      }

      return;
    }

    default: {
      static const unordered_map<string, vector<TokenType>> operations = {
          {IntType::name,
           {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH,
            TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
          {FloatType::name,
           {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH,
            TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
          {BooleanType::name, {TOKEN_AND, TOKEN_OR}},
          {StringType::name, {}},
          {PointerType::name, {}},
          {NullType::name, {}}};

      auto types_it = operations.find(left_type->get_name());
      if (types_it == operations.end()) {
        Logger::error("Unsupported type for binary expression",
                      LogTypes::Error::TYPE_MISMATCH, &position);
        return;
      }

      auto type_operations = operations.at(left_type->get_name());
      auto operations_it =
          find(type_operations.begin(), type_operations.end(), op.type);
      if (operations_it == type_operations.end()) {
        Logger::error(
            "Unsupported operation for type `" + left_type->to_string() + "`",
            LogTypes::Error::SYNTAX, &position);
        return;
      }
    }
  }
}

void BinaryExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  left->resolve_types(context);
  right->resolve_types(context);

  switch (op.type) {
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_THAN:
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
      set_type(make_shared<BooleanType>());
      break;

    case TOKEN_ASSIGN: {
      set_type(right->get_type());
      break;
    }

    case TOKEN_AS: {
      if (auto type_expression = dynamic_pointer_cast<TypeExpression>(right)) {
        set_type(type_expression->get_type());

        left->set_type(type);
      } else {
        Logger::error("Expected type expression as right hand side expression");
        return;
      }

      break;
    }

    default: {
      auto left_type = left->get_type();
      auto right_type = right->get_type();
      if (TypeHelper::is_float(left_type)) {
        set_type(left_type);
      } else {
        set_type(right_type);
      }
    }
  }
}

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(type: " + (type ? type->to_string_tree() : "") +
         ", op: " + op.literal + ", left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
