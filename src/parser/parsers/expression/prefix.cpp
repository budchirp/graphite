#include <memory>

#include "ast/expression/prefix.hpp"
#include "parser/parsers/expression/prefix.hpp"
#include "parser/parsers/statement/expression.hpp"

unique_ptr<Expression> PrefixExpressionParser::parse() {
  const auto prefix_token = parser->current_token;
  parser->eat_token(); // eat prefix

  auto right = ExpressionStatementParser(parser).parse_expression(
      PrecedenceHelper::precedence_for(prefix_token.type));  if (!right) {
    parser->get_logger()->error(
        "Failed to parse the right hand side of the prefix expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<PrefixExpression>(*right->get_position(), right->get_type(), prefix_token,
                                       std::move(right));
}
