#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/parser/parsers/expression/group.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/precedence.hpp"
#include "logger/log_types.hpp"

GroupExpressionParser::GroupExpressionParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> GroupExpressionParser::parse() {
  parser->eat_token(); // eat (

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    parser->logger->error("Expected right parentheses after expression",
                          LogTypes::Error::SYNTAX);
  }

  parser->eat_token(); // eat )

  return expression;
}
