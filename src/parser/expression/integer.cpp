#include "parser/expression/integer.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"

unique_ptr<Expression> IntegerExpressionParser::parse() {
  return parse_integer();
}

unique_ptr<IntegerExpression> IntegerExpressionParser::parse_integer() const {
  const auto position = *parser->get_lexer()->position;

  const auto integer_token = parser->current_token;
  parser->eat_token();  // eat integer

  return make_unique<IntegerExpression>(position, integer_token.literal);
}
