#include <memory>

#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/group.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"

unique_ptr<Expression> GroupExpressionParser::parse() {
  parser->eat_token(); // eat (

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    parser->get_logger()->error("Expected right parentheses after expression",
                          LogTypes::Error::SYNTAX);
  }

  parser->eat_token(); // eat )

  return expression;
}
