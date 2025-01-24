#include "ast/expression/binary.hpp"

#include <memory>

#include "analyzer/analyzer.hpp"
#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/binary.hpp"
#include "parser/parsers/expression/type.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "token/token_type.hpp"
#include "types/boolean.hpp"
#include "types/type.hpp"

unique_ptr<Expression> BinaryExpressionParser::parse() {
  const auto operator_token = parser->current_token;
  parser->eat_token();  // eat operator

  if (operator_token.type == TOKEN_AS) {
    parser->get_logger()->warn(
        "The use of the `as` keyword is discouraged, as there is no assurance "
        "that the specified type will match the type of the expression.",
        LogTypes::Warn::SUGGESTION);

    auto type = TypeExpressionParser(parser).parse();

    return make_unique<BinaryExpression>(*left->get_position(),
                                         type->get_type(), operator_token,
                                         std::move(left), std::move(type));
  }

  auto right = ExpressionStatementParser(parser).parse_expression(
      PrecedenceHelper::precedence_for(operator_token.type));
  if (!right) {
    parser->get_logger()->error(
        "Failed to parse the right hand side of the binary expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  shared_ptr<Type> type;
  switch (operator_token.type) {
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_THAN:
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
      type = make_shared<BooleanType>();
      break;

    case TOKEN_ASSIGN: {
      if (auto variable_expression =
              dynamic_cast<VarRefExpression *>(left.get())) {
        auto variable = parser->get_program()->get_env()->get_variable(
            variable_expression->get_name());

        if (variable->is_initialized && !variable->is_mutable) {
          parser->get_logger()->error("Cannot mutate an immutable variable",
                                      LogTypes::Error::SYNTAX);
          return nullptr;
        }

        if (!variable->is_initialized &&
            (!Analyzer::is_null(variable->actual_type).first ||
             !Analyzer::is_null(
                  Analyzer::is_pointer(variable->actual_type).second)
                  .first)) {
          shared_ptr<Type> type;
          if (auto pointer_type = Analyzer::is_pointer(variable->type);
              pointer_type.first) {
            if (auto null_type =
                    Analyzer::is_null(pointer_type.second->pointee_type);
                null_type.first) {
              type = make_shared<PointerType>(null_type.second->child_type);
            }
          } else if (auto null_type = Analyzer::is_null(variable->type);
                     null_type.first) {
            type = null_type.second->child_type;
          } else {
            type = variable->type;
          }

          variable->set_type(type);
        }
      } else {
        parser->get_logger()->error(
            "Expected variable reference as left hand side expression",
            LogTypes::Error::SYNTAX);
        return nullptr;
      }

      break;
    }

    default:
      type = left->get_type();
      break;
  }

  return make_unique<BinaryExpression>(*left->get_position(), type,
                                       operator_token, std::move(left),
                                       std::move(right));
}
