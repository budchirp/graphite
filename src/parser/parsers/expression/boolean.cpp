#include "ast/expression/boolean.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "parser/parsers/expression/boolean.hpp"

unique_ptr<Expression> BooleanExpressionParser::parse() {
  return parse_boolean();
}

unique_ptr<BooleanExpression> BooleanExpressionParser::parse_boolean() {
  const auto position = *parser->get_lexer()->position;

  const auto boolean_token = parser->current_token;
  parser->eat_token();  // eat boolean

  return make_unique<BooleanExpression>(position,
                                        boolean_token.literal == "true");
}
