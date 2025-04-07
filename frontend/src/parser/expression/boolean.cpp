#include "parser/expression/boolean.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/boolean.hpp"

shared_ptr<Expression> BooleanExpressionParser::parse() {
  return parse_boolean();
}

shared_ptr<BooleanExpression> BooleanExpressionParser::parse_boolean() const {
  const auto position = *parser->get_lexer()->position;

  const auto boolean_token = parser->current_token;
  parser->eat_token();  // eat boolean

  return make_shared<BooleanExpression>(position,
                                        boolean_token.literal == "true");
}
