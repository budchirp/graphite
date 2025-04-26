#include "parser/expression/group.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> GroupExpressionParser::parse() {
  parser->eat_token();  // eat (

  auto expression = ExpressionStatementParser(parser).parse_expression(
      Precedence::Value::LOWEST);
  if (!expression) {
    parser->get_logger()->error("Failed to parse expression",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    parser->get_logger()->error("Expected right parentheses after expression",
                                LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat )

  return expression;
}
