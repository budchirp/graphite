#include "parser/expression/binary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/binary.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/type.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> BinaryExpressionParser::parse() {
  const auto operator_token = parser->current_token;
  parser->eat_token();  // eat operator

  if (operator_token.type == TOKEN_AS) {
    parser->get_logger()->warn(
        "The use of the `as` keyword is discouraged, as there is no assurance "
        "that the specified type will match the type of the expression.",
        LogTypes::Warn::SUGGESTION);

    auto type = TypeExpressionParser(parser).parse();

    return make_shared<BinaryExpression>(*left->get_position(), operator_token,
                                         left, type);
  }

  auto right = ExpressionStatementParser(parser).parse_expression(
      Precedence::precedence_for(operator_token.type));
  if (!right) {
    parser->get_logger()->error(
        "Failed to parse the right hand side of the binary expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<BinaryExpression>(*left->get_position(), operator_token,
                                       left, right);
}
