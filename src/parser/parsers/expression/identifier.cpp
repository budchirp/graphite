#include "ast/expression/identifier.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "parser/parsers/expression/identifier.hpp"

unique_ptr<Expression> IdentifierExpressionParser::parse() {
  return parse_identifier();
}

unique_ptr<IdentifierExpression>
IdentifierExpressionParser::parse_identifier() {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  return make_unique<IdentifierExpression>(position, identifier_token.literal);
}
