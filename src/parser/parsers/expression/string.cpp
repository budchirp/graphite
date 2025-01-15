#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/string.hpp"
#include "parser/parsers/expression/string.hpp"
#include "types/string.hpp"

unique_ptr<Expression> StringExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;


  const auto string_token = parser->current_token;
  parser->eat_token(); // eat string

  return make_unique<StringExpression>(position,
                                       make_shared<StringType>(),
                                       string_token.literal);
}
