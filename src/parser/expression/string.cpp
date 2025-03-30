#include "parser/expression/string.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/string.hpp"

unique_ptr<Expression> StringExpressionParser::parse() {
  return parse_string();
}

unique_ptr<StringExpression> StringExpressionParser::parse_string() const {
  const auto position = *parser->get_lexer()->position;

  const auto string_token = parser->current_token;
  parser->eat_token();  // eat string

  return make_unique<StringExpression>(position, string_token.literal);
}
