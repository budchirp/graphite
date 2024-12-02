#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/parser/parsers/expression/group.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/precedence.hpp"
#include "utils/logger/logger.hpp"

GroupExpressionParser::GroupExpressionParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<Expression> GroupExpressionParser::parse() {
  parser->eat_token(); // eat (

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    Logger::error("Expected right parantheses after expression");
  }

  parser->eat_token(); // eat )

  return expression;
}
