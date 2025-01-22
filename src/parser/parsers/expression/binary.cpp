#include "ast/expression/binary.hpp"

#include <memory>

#include "ast/expression.hpp"
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
                                         std::move(left), nullptr);
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

    default:
      type = left->get_type();
      break;
  }

  return make_unique<BinaryExpression>(*left->get_position(), type,
                                       operator_token, std::move(left),
                                       std::move(right));
}
