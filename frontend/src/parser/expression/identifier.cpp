#include "parser/expression/identifier.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"

shared_ptr<Expression> IdentifierExpressionParser::parse() {
  return parse_identifier();
}

shared_ptr<IdentifierExpression> IdentifierExpressionParser::parse_identifier()
    const {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  return make_shared<IdentifierExpression>(position, identifier_token.literal);
}
