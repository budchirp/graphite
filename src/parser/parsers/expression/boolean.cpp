#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/boolean.hpp"
#include "parser/parsers/expression/boolean.hpp"
#include "types/boolean.hpp"

unique_ptr<Expression> BooleanExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  const auto boolean_token = parser->current_token;
  parser->eat_token(); // eat boolean

  return make_unique<BooleanExpression>(
      position, make_shared<BooleanType>(),
      boolean_token.literal == "true");
}
