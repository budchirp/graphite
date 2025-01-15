#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "parser/parsers/expression/identifier.hpp"

unique_ptr<Expression> IdentifierExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token(); // eat identifier

  return make_unique<IdentifierExpression>(position,
                                           parser->get_env()->get_symbol(identifier_token.literal),
                                           identifier_token.literal);
}
