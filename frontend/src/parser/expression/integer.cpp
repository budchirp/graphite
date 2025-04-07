#include "parser/expression/integer.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"

shared_ptr<Expression> IntegerExpressionParser::parse() {
  return parse_integer();
}

shared_ptr<IntegerExpression> IntegerExpressionParser::parse_integer() const {
  const auto position = *parser->get_lexer()->position;

  const auto integer_token = parser->current_token;
  parser->eat_token();  // eat integer

  return make_shared<IntegerExpression>(position, integer_token.literal);
}
